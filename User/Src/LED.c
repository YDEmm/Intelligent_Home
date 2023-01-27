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
 * @description: LED�Ƴ�ʼ��
 *               LED1------PB1
 *               LED2------PC5
 * @return {*}
 */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��B��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //ʹ��C��ʱ��
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_5;                //ѡ��ܽ�1  5
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;        //����ٶ�Ϊ50MHZ
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
    //PB1
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_SetBits(GPIOB, GPIO_Pin_1);          //����PB1��ʼ��ƽΪ�ߵ�ƽ
    //PC5
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_SetBits(GPIOC, GPIO_Pin_5);          //����PC5��ʼ��ƽΪ�ߵ�ƽ

}

#elif 0

#endif
