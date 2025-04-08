#include "uart_dma.h"
#include "scheduler.h"
#include "GPIO.h"
#include "servo.h"
#include "switch_async.h"

#define START_BYTE ((uint8_t)0xAA)
#define END_BYTE ((uint8_t)0x55)

#define GET_PARKING_SPOT_CMD ((uint8_t)0x11)
#define OPEN_ENTRY_GATE_CMD ((uint8_t)0x22)
#define CLOSE_ENTRY_GATE_CMD ((uint8_t)0x44)
#define OPEN_EXIT_GATE_CMD ((uint8_t)0x33)
#define CLOSE_EXIT_GATE_CMD ((uint8_t)0x66)

uint8_t buffTx[4] = {0};
uint8_t buffRx[4] = {0};

volatile uint8_t GarageStatus = 0;

uint8_t calcCheckSumRx(void)
{
    return (0 ^ buffRx[0] ^ buffRx[1]);
}

void encodeTxBuff(uint8_t data)
{
    buffTx[0] = START_BYTE;
    buffTx[1] = data;
    buffTx[2] = calcCheckSumTx();
    buffTx[3] = END_BYTE;
}

void getGarageInfo(void)
{
    uint8_t parkingSpotsStatus1,parkingSpotsStatus2,parkingSpotsStatus3,EntryGateStatus,ExitGateStatus;
    SwitchAsync_enuGetState(SWITCH1, &parkingSpotsStatus1);
    SwitchAsync_enuGetState(SWITCH2, &parkingSpotsStatus2);
    SwitchAsync_enuGetState(SWITCH3, &parkingSpotsStatus3);
    /*Get entry and exit status */
    GarageStatus = parkingSpotsStatus1 | (parkingSpotsStatus2 << 1) | (parkingSpotsStatus3 << 2) | (EntryGateStatus < 3) | (EntryGateStatus < 4);
}

void UartDMA_vRxCallBack(DMA_enuEvents_t event)
{
    if (event == DMA_enuOnFullTransfer)
    {
        if (buffRx[0] == START_BYTE && buffRx[3] == END_BYTE)
        {
            uint8_t checkSum = calcCheckSumRx();
            if (checkSum == buffRx[2])
            {
                switch (buffRx[1])
                {
                case GET_PARKING_SPOT_CMD:
                    encodeTxBuff(GarageStatus);
                    UartDMA_enuStartTransfer(UART_LINE_1);
                    break;

                case OPEN_ENTRY_GATE_CMD:
                    Servo_enuSetAngle(SERVO_1, 90);
                    break;

                case CLOSE_ENTRY_GATE_CMD:
                    Servo_enuSetAngle(SERVO_1, 0);
                    break;

                case OPEN_EXIT_GATE_CMD:
                    Servo_enuSetAngle(SERVO_2, 90);
                    break;
                
                case CLOSE_EXIT_GATE_CMD:
                    Servo_enuSetAngle(SERVO_2, 0);
                    break;

                default:
                    break;
                }
            }
        }
        UartDMA_enuStartTransfer(UART_LINE_2);
    }
}

SCHED_stTaskCfg_t taskCfg = {
    .CallBack = getGarageInfo,
    .CycleTime = 500,
    .Priority = 0};

int main(void)
{
    UartDMA_vInit();
    UartDMA_enuRegisterBuff(UART_LINE_1, buffTx, 4);
    UartDMA_enuRegisterBuff(UART_LINE_2, buffRx, 4);
    UartDMA_enuRegisterCallBack(UART_LINE_2, UartDMA_vRxCallBack);
    UartDMA_enuStartTransfer(UART_LINE_2);
    SCHED_enuAddRunnable(&taskCfg);
    SCHED_vInit();
    SCHED_vStart();
    return 0;
}