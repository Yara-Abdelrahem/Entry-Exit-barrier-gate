#ifndef RCC_H_
#define RCC_H_
#include "std_types.h"

#define RCC_CLK_STATE_ON        0xFFFFFFFFUL
#define RCC_CLK_STATE_OFF       0x00000000UL

#define RCC_CLK_HSI             0x00000001UL                 
#define RCC_CLK_HSE             0x00010000UL
#define RCC_CLK_PLL             0x01000000UL

#define RCC_CLK_RDY             ((uint8_t)0x1)
#define RCC_CLK_NOT_RDY         ((uint8_t)0x0)

#define RCC_HSE_BY_ON           ((uint8_t)0x1)
#define RCC_HSE_BY_OFF          ((uint8_t)0x0)

#define RCC_PER_ON              0xFFFFFFFFUL
#define RCC_PER_OFF             0x00000000UL

/*AHB1 Peripherals*/
#define RCC_GPIOA               0x00000001ULL
#define RCC_GPIOB               0x00000002ULL
#define RCC_GPIOC               0x00000004ULL
#define RCC_GPIOD               0x00000008ULL
#define RCC_GPIOE               0x00000010ULL
#define RCC_GPIOH               0x00000080ULL
#define RCC_CRC                 0x00001000ULL
#define RCC_DMA1                0x00200000ULL
#define RCC_DMA2                0x00400000ULL

/*AHB2 Peripherals*/
#define RCC_OTGFS               0x100000080ULL

/*APB1 Peripherals*/
#define RCC_TIM2                0x200000001ULL
#define RCC_TIM3                0x200000002ULL
#define RCC_TIM4                0x200000004ULL
#define RCC_TIM5                0x200000008ULL
#define RCC_WDG                 0x200000800ULL
#define RCC_SPI2                0x200004000ULL
#define RCC_SPI3                0x200008000ULL
#define RCC_USART2              0x200020000ULL
#define RCC_I2C1                0x200200000ULL
#define RCC_I2C2                0x200400000ULL
#define RCC_I2C3                0x200800000ULL
#define RCC_PWR                 0x210000000ULL

/*APB2 Peripherals*/
#define RCC_TIM1                0x400000001ULL
#define RCC_USART1              0x400000010ULL
#define RCC_USART6              0x400000020ULL
#define RCC_ADC1                0x400000100ULL
#define RCC_SDIO                0x400000800ULL
#define RCC_SPI1                0x400001000ULL
#define RCC_SPI4                0x400002000ULL
#define RCC_SYSCFG              0x400004000ULL
#define RCC_TIM9                0x400010000ULL
#define RCC_TIM10               0x400020000ULL
#define RCC_TIM11               0x400040000ULL

/*AHB Prescaler inputs*/
#define RCC_AHB_PRE_NO_PRE      0
#define RCC_AHB_PRE_2           1
#define RCC_AHB_PRE_4           2
#define RCC_AHB_PRE_8           3
#define RCC_AHB_PRE_16          4
#define RCC_AHB_PRE_64          5
#define RCC_AHB_PRE_128         6
#define RCC_AHB_PRE_256         7
#define RCC_AHB_PRE_512         8

/*APB1 prescaler inputs*/
#define RCC_APB1_NO_PRE         0
#define RCC_APB1_PRE_2          1
#define RCC_APB1_PRE_4          2
#define RCC_APB1_PRE_8          3
#define RCC_APB1_PRE_16         4

/*APB2 Prescaler inputs*/
#define RCC_APB2_NO_PRE         0
#define RCC_APB2_PRE_2          1
#define RCC_APB2_PRE_4          2
#define RCC_APB2_PRE_8          3
#define RCC_APB2_PRE_16         4

/*Error status*/
typedef enum{
    RCC_enu_OK,
    RCC_enu_INVALID_CLK_TYPE,
    RCC_enu_INVALID_CLK_STATE,
    RCC_enu_CLK_TIMEOUT,
    RCC_enu_NULL_POINTER,
    RCC_enu_PLL_ON,
    RCC_enu_HSE_ON,
    RCC_enu_INVALID_PLL_CFG,
    RCC_enu_INVALID_PERIPHERAL,
    RCC_enu_INVALID_PRESCALER,
    RCC_enu_INVALID_BUS,
    RCC_enu_NOK
}RCC_enuErrorStatus_t;

/*Bus inputs*/
typedef enum{
    RCC_enu_AHB,
    RCC_enu_APB1,
    RCC_enu_APB2
}RCC_enuBuses_t;

/*PLL parametrs input*/
typedef struct {
    uint32_t clk;
    uint8_t M;
    uint16_t N;
    uint8_t P;
    uint8_t Q;
}RCC_stPllParams_t;

extern RCC_enuErrorStatus_t RCC_enuCtlClk(uint32_t Copy_u32Clk,uint32_t Copy_u32State);
 
extern RCC_enuErrorStatus_t RCC_enuClkIsRdy(uint32_t Copy_u32Clk,uint8_t* Add_u8Ret);

extern RCC_enuErrorStatus_t RCC_selCLK(uint32_t Copy_u32Clk);

extern uint32_t RCC_enuGetClkFreq(uint32_t Copy_u32Clk,uint32_t* Add_u32ClkFreq);

extern RCC_enuErrorStatus_t RCC_enuCFGPLL(RCC_stPllParams_t* Add_stParams);

extern RCC_enuErrorStatus_t RCC_enuBypassHSE(uint8_t Copy_u8State);
 
extern uint8_t RCC_enuIsHSEBYP(void);

extern RCC_enuErrorStatus_t RCC_enuCtlPeripheral(uint64_t Copy_u64Per,
uint32_t Copy_u8State);

extern uint32_t RCC_enuGetSysClk(void);

extern RCC_enuErrorStatus_t RCC_enuSetBusPrescaler(RCC_enuBuses_t Copy_enuBus,
uint32_t Copy_u32Prescaler);

extern RCC_enuErrorStatus_t RCC_getBusFreq(RCC_enuBuses_t Copy_enuBus,
uint32_t* Add_u64Freq);

#endif