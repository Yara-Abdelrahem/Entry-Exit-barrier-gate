#ifndef LCD_CFG_H_
#define LCD_CFG_H_
#include "std_types.h"
#include "GPIO.h"
#include "scheduler.h"
#include "rcc.h"

#ifndef LCD_MODE
#define LCD_MODE            LCD_8_BIT_MODE
#endif

#if (LCD_MODE == LCD_8_BIT_MODE)
#define LCD_NO_OF_TOTAL_PINS    11
#define LCD_NO_OF_DATA_PINS     8
#else
#define LCD_NO_OF_TOTAL_PINS    7
#define LCD_NO_OF_DATA_PINS     4
#endif 

typedef struct{
    GPIO_enuPorts_t port;
    GPIO_enuPins_t pin;
}LCD_stPins_t;

typedef struct{
    LCD_stPins_t RS;
    LCD_stPins_t RW;
    LCD_stPins_t EN;
    LCD_stPins_t data[LCD_NO_OF_DATA_PINS];
    uint16_t FunSet;
    uint16_t EntryMode;
    uint16_t DisplaySettings;
    uint32_t RunnablePriority;
}LCD_stCfg_t;

#endif