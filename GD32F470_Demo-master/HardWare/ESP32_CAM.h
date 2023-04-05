#ifndef __ESP32_CAM_H__
#define __ESP32_CAM_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
#include <stdio.h>
/* eval board low layer led */

#define COMn                             1U
#define ESP32_CAM_COM0                        USART2
#define ESP32_CAM_COM0_CLK                    RCU_USART2

#define ESP32_CAM_COM0_TX_PIN                 GPIO_PIN_10
#define ESP32_CAM_COM0_RX_PIN                 GPIO_PIN_11

#define ESP32_CAM_COM0_GPIO_PORT              GPIOB
#define ESP32_CAM_COM0_GPIO_CLK               RCU_GPIOB
#define ESP32_CAM_COM0_AF                     GPIO_AF_7


extern char USART2_Tx_buffer[256];
extern char USART2_Rx_buffer[256];
/* function declarations */
/* configure COM port */
void ESP32_CAM_USART2_Init(uint32_t com);

void USART2_Send_Byte(uint8_t ch);
void USART2_Send_String(uint8_t *str);

#ifdef __cplusplus
}
#endif

#endif /* __ESP32_CAM_H__ */
