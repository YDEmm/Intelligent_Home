/*
 * @Author: JYM
 * @Date: 2023-01-20 20:08:40
 * @LastEditTime: 2023-01-24 17:21:31
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\User\Src\beep.c
 * 
 */
#include "beep.h"
#if 1

/**
 * @description: BEEP初始化
 *               BEEP------PC3
 * @return {*}
 */
void BEEP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能C口时钟
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;                //选择管脚1  5
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;        //输出速度为50MHZ
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    //PC3
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_ResetBits(GPIOC, GPIO_Pin_3);          //设置PC3初始电平为低电平
}


#elif 0

#endif

