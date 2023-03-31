#ifndef __UART4_LCD_H__
#define __UART4_LCD_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
#include <stdio.h>
/* eval board low layer led */

#define COMn                             1U
#define LCD_COM0                        UART4
#define LCD_COM0_CLK                    RCU_UART4

#define LCD_COM0_TX_PIN                 GPIO_PIN_12   //PC12
#define LCD_COM0_RX_PIN                 GPIO_PIN_2    //PD2

#define LCD_COM0_TX_PORT                GPIOC
#define LCD_COM0_TX_CLK                 RCU_GPIOC
#define LCD_COM0_RX_PORT                GPIOD
#define LCD_COM0_RX_CLK                 RCU_GPIOD
#define LCD_COM0_AF                     GPIO_AF_8


typedef struct Uart_rec Lcd_uart; //在 ZigBee_UART3.h 中定义
extern Lcd_uart Lcd_Rec;

void UART4_Lcd_Init(uint32_t com);

void Lcd_Cmd(char *buf);
int Lcd_Read(int waittime,char str[]);


#endif


