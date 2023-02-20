/*** 
 * @Author: JYM
 * @Date: 2023-01-20 20:08:40
 * @LastEditTime: 2023-01-24 17:24:50
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\User\Inc\LED.h
 * @
 */
#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h"                  // Device header


#define LED1_ON    (GPIO_ResetBits(GPIOA, GPIO_Pin_4))
#define LED1_OFF    (GPIO_SetBits(GPIOA, GPIO_Pin_4))
#define LED0_ON    (GPIO_ResetBits(GPIOC, GPIO_Pin_13))
#define LED0_OFF    (GPIO_SetBits(GPIOC, GPIO_Pin_13))


void LED_Init(void);




#endif



