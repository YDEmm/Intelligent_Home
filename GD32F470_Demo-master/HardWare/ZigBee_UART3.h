#ifndef __ZigBee_UART3_H__
#define __ZigBee_UART3_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
#include <stdio.h>
#include <string.h>
/* eval board low layer UART*/

#define COMn                             1U
#define ZigBee_COM4                        UART3
#define ZigBee_COM4_CLK                    RCU_UART3
        
#define ZigBee_COM4_TX_PIN                 GPIO_PIN_10
#define ZigBee_COM4_RX_PIN                 GPIO_PIN_11
        
#define ZigBee_COM4_GPIO_PORT              GPIOC
#define ZigBee_COM4_GPIO_CLK               RCU_GPIOC
#define ZigBee_COM4_AF                     GPIO_AF_8

#define RX_BUF_MAX_LEN                     1024
typedef struct Uart_rec
{
	int Rec_Start;						//开始接收标志
	int Rec_Finsh;						//结束接受标志
	
	int Rec_Num;						//接收字符数
	char Rec_String[RX_BUF_MAX_LEN];	//接收字符串
	char *Rec_Str;						//接收字符串

	int Rec_Time;						//超时时间
	int Rec_Timeflag;                   //理想超时时间
		
}Zigbee_Uart;



extern uint8_t ZigBee_Send_Flag;
extern uint8_t ZigBee_Recvice_Flag;

extern char UART3_Tx_DMA_buffer[256];
extern char UART3_Rx_DMA_buffer[256];

extern uint32_t Rx_DMA_BuffNum;
extern uint8_t ZigBee_Send_Flag;
extern char Subg_UART3_Rx_Data[];

/* function declarations */
/* configure COM port */
void ZigBee_UART3_init(uint32_t com);
void UART3_DMA_Send(char *buffer);


#ifdef __cplusplus
}
#endif

#endif /* __ZigBee_UART3_H__ */
