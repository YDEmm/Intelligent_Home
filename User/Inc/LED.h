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


#define LED1_ON    (GPIO_ResetBits(GPIOB, GPIO_Pin_1))
#define LED1_OFF    (GPIO_SetBits(GPIOB, GPIO_Pin_1))
#define LED2_ON    (GPIO_ResetBits(GPIOC, GPIO_Pin_5))
#define LED2_OFF    (GPIO_SetBits(GPIOC, GPIO_Pin_5))


void LED_Init(void);




#endif



