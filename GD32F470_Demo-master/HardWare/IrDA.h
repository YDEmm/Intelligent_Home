#ifndef __IRDA_H__
#define __IRDA_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
#include <stdio.h>


#define COMn                             1U
#define IrDA_COM0                        USART5
#define IrDA_COM0_CLK                    RCU_USART5
        
#define IrDA_COM0_TX_PIN                 GPIO_PIN_6   //PC6
#define IrDA_COM0_RX_PIN                 GPIO_PIN_7    //PC7
        
#define IrDA_COM0_GPIO_PORT              GPIOC
#define IrDA_COM0_GPIO_CLK               RCU_GPIOC
#define IrDA_COM0_AF                     GPIO_AF_8

extern int Confirm_Flag;

void USART5_IrDA_Init(uint32_t com);
void Send_IrDA_String(uint8_t *str);

#endif   //__IRDA_H__



