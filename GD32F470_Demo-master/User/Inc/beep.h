/*** 
 * @Author: JYM
 * @Date: 2023-01-20 20:08:40
 * @LastEditTime: 2023-01-24 17:25:03
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\User\Inc\beep.h
 * @
 */
#ifndef __BEEP_H__
#define __BEEP_H__

#include "stm32f10x.h"                  // Device header

#define BEEP_ON   (GPIO_SetBits(GPIOA, GPIO_Pin_8))
#define BEEP_OFF    (GPIO_ResetBits(GPIOA, GPIO_Pin_8))


void BEEP_Init(void);




#endif



