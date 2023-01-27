/*
 * @Author: JYM
 * @Date: 2023-01-20 20:08:40
 * @LastEditTime: 2023-01-24 23:09:55
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\User\Src\main.c
 * 
 */
#include "main.h"
#include "function.h"

/***************************************************
 * USART1:PA9 PA10   USART2:PA2 PA3
 * LED1: PA5 L
 * BEEP:PA6 H
 * DHT11:PB9 H
 * GY30:   PB10->SCL SDA->PB11 ADDR->GND
 * MQ-2:    PA1->A0-----模拟量输出接口
 **************************************************/
int main()
{
    TIM3_Init(5000, 7200);   //0.5s 进一次中断
    Usart1_Init(115200);
    DHT11_Init();
    IIC_Init();
    BH1750_Init();
    ADC_MQ_2_Init();
    Step_Motor_Init();
    Relay_Init();
    
    while(1)
    {
#if 1   
        //Step_Motor_Test();
		Relay_Loop();
        
        
        
        
#endif
        
        
    }
        
}


