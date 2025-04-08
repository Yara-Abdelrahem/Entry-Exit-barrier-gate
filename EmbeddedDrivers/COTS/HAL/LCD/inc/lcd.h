#ifndef LCD_H_
#define LCD_H_
#include "lcd_cfg.h"

#define LCD_RUNNABLE_PRIO   21

#define LCD_CLR_DISP_CMD                ((uint16_t)0x0001)
#define LCD_RET_HOME_CMD                ((uint16_t)0x0002)

#define LCD_ENT_CUR_LEFT                ((uint16_t)0x0004)
#define LCD_ENT_SCR_LEFT                ((uint16_t)0x0005)
#define LCD_ENT_CUR_RIGHT               ((uint16_t)0x0006)
#define LCD_ENT_SCR_RIGHT               ((uint16_t)0x0007)

#define LCD_DISP_OFF                    ((uint16_t)0x0008) 
#define LCD_DISP_ON                     ((uint16_t)0x000C) 
#define LCD_CUR                         ((uint16_t)0x000E) 
#define LCD_BLINK                       ((uint16_t)0x000F)

#define LCD_FUNC_SET_8BIT_2LINE_5x8     ((uint16_t)0x0038)
#define LCD_FUNC_SET_8BIT_1LINE_5x8     ((uint16_t)0x0030)
#define LCD_FUNC_SET_8BIT_2LINE_5x10    ((uint16_t)0x003C)
#define LCD_FUNC_SET_8BIT_1LINE_5x10    ((uint16_t)0x0034)
#define LCD_FUNC_SET_4BIT_2LINE_5x8     ((uint16_t)0x0028)
#define LCD_FUNC_SET_4BIT_1LINE_5x8     ((uint16_t)0x0020)
#define LCD_FUNC_SET_4BIT_2LINE_5x10    ((uint16_t)0x002C)
#define LCD_FUNC_SET_4BIT_1LINE_5x10    ((uint16_t)0x0024)

typedef enum{
    LCD_enuOK,
    LCD_enuINVALID_CMD,
    LCD_enuNOK
}LCD_enuErrorstatus_t;

#endif