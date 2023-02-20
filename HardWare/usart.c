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
 * @description: printf底层函数
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
 * @description: 串口发送数据函数
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
 * @description: 串口1初始化函数
 * 				 PA9 -- USART1_TX -- 复用推挽
 * 				 PA10 -- USART1_RX -- 浮空输入
 * @param {u32} baud
 * @return {*}
 */
void Usart1_Init(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*1.开启PA口和串口1时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);
	/*2.配置引脚工作模式*/
	//PA9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//PA10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*3.配置串口基础*/
	USART_InitStructure.USART_BaudRate = baud;                                         //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                        //数据位位宽
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                             //停止位位宽
	USART_InitStructure.USART_Parity = USART_Parity_No;                                //不使用奇偶校验
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;     //不使用硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                    //使能收发器
	USART_Init(USART1, &USART_InitStructure);
	/*4.设置中断源*/
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	/*5.配置中断源优先级*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/*6.开启串口*/
	USART_Cmd(USART1,ENABLE);
}


/**
 * @description: 串口1中断服务函数
 * @return {*}
 */
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}



