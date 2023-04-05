#ifndef __USART0_H__
#define __USART0_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
#include <stdio.h>
/* eval board low layer led */

#define COMn                             1U
#define EVAL_COM0                        USART0
#define EVAL_COM0_CLK                    RCU_USART0

#define EVAL_COM0_TX_PIN                 GPIO_PIN_9
#define EVAL_COM0_RX_PIN                 GPIO_PIN_10

#define EVAL_COM0_GPIO_PORT              GPIOA
#define EVAL_COM0_GPIO_CLK               RCU_GPIOA
#define EVAL_COM0_AF                     GPIO_AF_7


extern char USART0_Tx_buffer[256];
extern char USART0_Rx_buffer[256];
/* function declarations */
/* configure COM port */
void gd_eval_com_init(uint32_t com);

void USART0_Send_Byte(uint8_t ch);
void USART0_Send_String(uint8_t *str);

#ifdef __cplusplus
}
#endif

#endif /* __USART0_H__ */
