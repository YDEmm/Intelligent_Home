/*
 * @Author: JYM
 * @Date: 2023-01-26 09:26:09
 * @LastEditTime: 2023-01-26 09:44:13
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\HardWare\relay.c
 * 
 */
#include "relay.h"                

/**
 * @description: relay��ʼ��
 *               DC+               5V
 *               DC-               GND
 *               IN                PB7
 * @return {*}
 */
void Relay_Init(void) 
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RELAY_CLK, ENABLE); //ʹ��B��ʱ��
    
    GPIO_InitStruct.GPIO_Pin = RELAY_PIN;                //ѡ��ܽ�7
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;        //����ٶ�Ϊ50MHZ
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
    //PB7
    GPIO_Init(RELAY_PORT, &GPIO_InitStruct);
}
/**
 * @description: relay
 * @return {*}
 */
void Relay_Loop(void) 
{
    GPIO_SetBits(RELAY_PORT, RELAY_PIN);          //����PB7Ϊ�ߵ�ƽ
    delay_ms(1000);//��ʱ1��
    GPIO_ResetBits(RELAY_PORT, RELAY_PIN);          //����PB7Ϊ�͵�ƽ
    delay_ms(1000);
}


