/*** 
 * @Author: JYM
 * @Date: 2023-01-26 09:26:26
 * @LastEditTime: 2023-01-26 09:44:27
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\HardWare\relay.h
 * @
 */
#ifndef __RELAY_H__
#define __RELAY_H__

#include "stm32f10x.h"                  // Device header
#include "Systic.h"

#define RELAY_CLK       RCC_APB2Periph_GPIOA
#define RELAY_PORT	    GPIOA	             //定义IO接口所在组
#define RELAY_PIN       GPIO_Pin_11

void Relay_Init(void);
void Relay_Loop(void); 



#endif

