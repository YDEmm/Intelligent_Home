/*** 
 * @Author: JYM
 * @Date: 2023-01-21 08:59:14
 * @LastEditTime: 2023-01-23 11:16:20
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \00jym\2.Project\JYM--DHT11\HardWare\usart.h
 * @
 */
#ifndef __USART_H_
#define __USART_H_

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <stdio.h>

extern u8 Rcv_flag;
extern u8 Rcv_Buff[100];
void Usart1_Init(u32 baud);
void Usart1_Send_Data(u8 *data);

#if 0
void Usart2_Init(u32 baud);
void Usart2_Send_Byte(u8 *data);
void Usart2_Send_Str(u8 *data);
#endif


#endif
