/*** 
 * @Author: JYM
 * @Date: 2023-01-20 20:31:16
 * @LastEditTime: 2023-01-24 17:25:19
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\HardWare\Systic.h
 * @
 */
#ifndef _SYSTIC_H
#define _SYSTIC_H

#include "stm32f10x.h"



void Systic_Init(void);
void delay_ms(u32 ms);
void delay_us(u32 us);

void Delay_us(u32 us);


#endif
