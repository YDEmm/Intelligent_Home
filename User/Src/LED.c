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
 *               LED1------PB1
 *               LED2------PC5
 * @return {*}
 */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能B口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能C口时钟
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_5;                //选择管脚1  5
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;        //输出速度为50MHZ
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    //PB1
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_SetBits(GPIOB, GPIO_Pin_1);          //设置PB1初始电平为高电平
    //PC5
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_SetBits(GPIOC, GPIO_Pin_5);          //设置PC5初始电平为高电平

}

#elif 0

#endif
