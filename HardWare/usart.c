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

#if 0
/**
PA2 -- USART2_TX -- ��������
PA3 -- USART2_RX -- ��������
FING_PWR --- PB9
*/
void Usart2_Init(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
	//ʹ��A��Bʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	//ʹ�ܴ���2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	/*MG200_PWR----PB9*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
	
	/*PA2 TX*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
		/*PA3 RX*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	USART_InitStruct.USART_BaudRate = baud;//������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��ҪӲ��������
	USART_InitStruct.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;//ʹ�ܷ���&&����
	USART_InitStruct.USART_Parity = USART_Parity_No;//����żУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//1λֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//һ����ʼλ 8������λ
	USART_Init(USART2,&USART_InitStruct);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//�����ж�ʹ��
	
	NVIC_SetPriorityGrouping(7-2);
	NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority (7-2,0,0));
	NVIC_EnableIRQ(USART2_IRQn);
	USART_Cmd(USART2,ENABLE);//ʹ�ܴ���
}
u8 Rcv_flag =0;     //�����±�
u8 Rcv_Buff[100];
/*****************����2�жϷ�����**********************/
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
	{
		TIM_SetCounter(TIM2,0);//����������
		if(Rcv_flag==0)
		{
			TIM_Cmd(TIM2,ENABLE);
		}
		Rcv_Buff[Rcv_flag++]=USART_ReceiveData(USART2);
	}
}

void Usart2_Send_Byte(u8 *data)
{
	while(*data != '\0')
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET)
		{
			
		}
		USART_SendData(USART2,*data);
		data++;
	}
}

void Usart2_Send_Str(u8 *data)
{
	while(*data != 0x00)
	{
		while( !(USART2->SR & (1 << 7)) )
		{
			/* �ȴ����ݷ������ */
		}
		USART2->DR = *data;
		data++;
	}	
}

#endif


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



