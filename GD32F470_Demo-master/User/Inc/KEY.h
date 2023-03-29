/*** 
 * @Author: JYM
 * @Date: 2023-01-20 20:08:40
 * @LastEditTime: 2023-01-24 17:25:11
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\User\Inc\KEY.h
 * @
 */
#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f10x.h"                  // Device header
#include "beep.h"

#define KEY     GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)



void KEY_Init(void);
u8 KEY_Scan(void);



#endif



