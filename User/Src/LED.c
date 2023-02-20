/*
 * @Author: JYM
 * @Date: 2023-01-20 20:08:40
 * @LastEditTime: 2023-01-24 17:20:06
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\User\Src\LED.c
 * 
 */
#include "LED.h"
#if 1

/**
 * @description: LED灯初始化
 *               LED1------PA4
 *               LED0------PC13
 * @return {*}
 */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能A口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能A口时钟
    
    //PA4
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;                //选择管脚4  5
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;        //输出速度为50MHZ
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出   
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_SetBits(GPIOA, GPIO_Pin_4);          //设置PA4初始电平为高电平
    
    //PC13
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;	    		 //LED1-->PE.5 端口配置, 推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStruct);	  				 //推挽输出 ，IO口速度为50MHz
    GPIO_SetBits(GPIOC,GPIO_Pin_13);          //设置PC13初始电平为高电平

}

#elif 0

#endif
