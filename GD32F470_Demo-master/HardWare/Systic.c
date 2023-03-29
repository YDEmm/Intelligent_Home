/*
 * @Author: JYM
 * @Date: 2023-01-20 20:31:31
 * @LastEditTime: 2023-01-24 17:23:38
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\HardWare\Systic.c
 * 
 */
#include "Systic.h"

/**
 * @description: �δ�ʱ����ʼ������
 * @return {*}
 */
void Systic_Init(void)
{	
	SysTick->CTRL &=~(1<<2);     //����ʹ���ⲿʱ��Դ			
}

/**
 * @description: ms��ʱ
 * @param {u32} ms
 * @return {*}
 */
void delay_ms(u32 ms)
{
	u32 temp;	    	 

	SysTick->LOAD = ms*72000/8;			//������װ��ֵ�����������¼�����
	SysTick->VAL	=	0x00;				//��ռ�����
	SysTick->CTRL |= 1<<0;   //ʹ��ϵͳ�δ�ʱ��

	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;      					 //��ռ�����		
}

/**
 * @description: us��ʱ
 * @param {u32} us
 * @return {*}
 */
void delay_us(u32 us)
{
	u32 temp;	    	 

	SysTick->LOAD = us*72/8;			//������װ��ֵ�����������¼�����
	SysTick->VAL	=	0x00;				//��ռ�����
	SysTick->CTRL |= 1<<0;   //ʹ��ϵͳ�δ�ʱ��

	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;      					 //��ռ�����	
			
	
}

/**
 * @description: us��ʱ
 * @param {u32} us
 * @return {*}
 */
void Delay_us(u32 us)
{
	u32 temp;	    	 

	SysTick->LOAD = us*72/8;			//������װ��ֵ�����������¼�����
	SysTick->VAL	=	0x00;				//��ռ�����
	SysTick->CTRL |= 1<<0;   //ʹ��ϵͳ�δ�ʱ��
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;      					 //��ռ�����		
}


