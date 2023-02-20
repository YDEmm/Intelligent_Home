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
 *               LED1------PA4
 *               LED0------PC13
 * @return {*}
 */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��A��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //ʹ��A��ʱ��
    
    //PA4
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;                //ѡ��ܽ�4  5
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;        //����ٶ�Ϊ50MHZ
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;         //�������   
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_SetBits(GPIOA, GPIO_Pin_4);          //����PA4��ʼ��ƽΪ�ߵ�ƽ
    
    //PC13
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;	    		 //LED1-->PE.5 �˿�����, �������
    GPIO_Init(GPIOC, &GPIO_InitStruct);	  				 //������� ��IO���ٶ�Ϊ50MHz
    GPIO_SetBits(GPIOC,GPIO_Pin_13);          //����PC13��ʼ��ƽΪ�ߵ�ƽ

}

#elif 0

#endif
