/*
 * @Author: JYM
 * @Date: 2023-01-21 08:58:58
 * @LastEditTime: 2023-01-24 17:19:30
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\HardWare\usart.c
 * 
 */
#include "usart.h"

/**
 * @description: printf�ײ㺯��
 * @param {int} data
 * @param {FILE} *file
 * @return {*}
 */
int fputc(int data,FILE *file)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET)
	{
		
	}
	USART_SendData(USART1,data);
	return data;
}

/**
 * @description: ���ڷ������ݺ���
 * @param {u8} *data
 * @return {*}
 */
void Usart1_Send_Data(u8 *data)
{
	while(*data != '\0')
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET)
		{
			
		}
		USART_SendData(USART1,*data);
		data++;
	}
}


/**
 * @description: ����1��ʼ������
 * 				 PA9 -- USART1_TX -- ��������
 * 				 PA10 -- USART1_RX -- ��������
 * @param {u32} baud
 * @return {*}
 */
void Usart1_Init(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*1.����PA�ںʹ���1ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);
	/*2.�������Ź���ģʽ*/
	//PA9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//PA10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*3.���ô��ڻ���*/
	USART_InitStructure.USART_BaudRate = baud;                                         //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                        //����λλ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                             //ֹͣλλ��
	USART_InitStructure.USART_Parity = USART_Parity_No;                                //��ʹ����żУ��
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;     //��ʹ��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                    //ʹ���շ���
	USART_Init(USART1, &USART_InitStructure);
	/*4.�����ж�Դ*/
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	/*5.�����ж�Դ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/*6.��������*/
	USART_Cmd(USART1,ENABLE);
}


/**
 * @description: ����1�жϷ�����
 * @return {*}
 */
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}



