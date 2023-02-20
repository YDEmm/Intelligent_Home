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
 * @description: BEEP��ʼ��
 *               BEEP------PA8
 * @return {*}
 */
void BEEP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��A��ʱ��
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;                //ѡ��ܽ�8
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;        //����ٶ�Ϊ50MHZ
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
    //PA8
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);          //����PA8��ʼ��ƽΪ�͵�ƽ
}


#elif 0

#endif

