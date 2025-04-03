#include "uart_dma.h"
#include "scheduler.h"
#include "GPIO.h"

#define START_BYTE                  ((uint8_t)0xAA)
#define END_BYTE                    ((uint8_t)0x55)

#define GET_PARKING_SPOT_CMD        ((uint8_t)0x11)
#define OPEN_ENTRY_GATE_CMD         ((uint8_t)0x22)
#define OPEN_EXIT_GATE_CMD          ((uint8_t)0x33)

uint8_t buffTx[4] = {0};
uint8_t buffRx[4] = {0};

volatile uint8_t parkingSpotsStatus = 3;

uint8_t calcCheckSumRx(void){
    return (buffRx[0] ^ buffRx[1]);
}

uint8_t calcCheckSumTx(void){
    return (buffTx[0] ^ buffTx[1]);
}

void encodeTxBuff(uint8_t data){
    buffTx[0] = START_BYTE;
    buffTx[1] = data;
    buffTx[2] = calcCheckSumTx();
    buffTx[3] = END_BYTE;
}

uint8_t getParkingSpots(void){
    return parkingSpotsStatus;
}

void checkdistance(void){
    /*check pulse values*/
    /*For first first ultrasonic*/
    /*if(pulse value < NO){
        sendTx(data)
    }*/

}

//(yy00 0xxx)

//Start byte -> 0xAA
// 1 -> someone at open
// 2 -> someone at exit
//checksum
//End byte -> 0x55

void UartDMA_vRxCallBack(DMA_enuEvents_t event){
    if(event == DMA_enuOnFullTransfer){
        if(buffRx[0] == START_BYTE && buffRx[3] == END_BYTE){
            uint8_t checkSum = calcCheckSumRx();
            if(checkSum == buffRx[2]){
                switch(buffRx[1]){
                    case GET_PARKING_SPOT_CMD:
                    encodeTxBuff(getParkingSpots());
                    UartDMA_enuStartTransfer(UART_LINE_1);
                    break;

                    case OPEN_ENTRY_GATE_CMD:
                    break;

                    case OPEN_EXIT_GATE_CMD:
                    break;
                    
                    default:
                    break;
                }
            }
        }
        UartDMA_enuStartTransfer(UART_LINE_2);
    }
}

int main(void){
    UartDMA_vInit();
    UartDMA_enuRegisterBuff(UART_LINE_1,buffTx,4);
    UartDMA_enuRegisterBuff(UART_LINE_2,buffRx,4);
    UartDMA_enuRegisterCallBack(UART_LINE_2,UartDMA_vRxCallBack);
    UartDMA_enuStartTransfer(UART_LINE_2);
    SCHED_vInit();
    SCHED_vStart();


    return 0;
}