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
 * @description: 滴答定时器初始化函数
 * @return {*}
 */
void Systic_Init(void)
{	
	SysTick->CTRL &=~(1<<2);     //配置使用外部时钟源			
}

/**
 * @description: ms延时
 * @param {u32} ms
 * @return {*}
 */
void delay_ms(u32 ms)
{
	u32 temp;	    	 

	SysTick->LOAD = ms*72000/8;			//设置重装载值（这玩意向下计数）
	SysTick->VAL	=	0x00;				//清空计数器
	SysTick->CTRL |= 1<<0;   //使能系统滴答定时器

	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器		
}

/**
 * @description: us延时
 * @param {u32} us
 * @return {*}
 */
void delay_us(u32 us)
{
	u32 temp;	    	 

	SysTick->LOAD = us*72/8;			//设置重装载值（这玩意向下计数）
	SysTick->VAL	=	0x00;				//清空计数器
	SysTick->CTRL |= 1<<0;   //使能系统滴答定时器

	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器	
			
	
}

/**
 * @description: us延时
 * @param {u32} us
 * @return {*}
 */
void Delay_us(u32 us)
{
	u32 temp;	    	 

	SysTick->LOAD = us*72/8;			//设置重装载值（这玩意向下计数）
	SysTick->VAL	=	0x00;				//清空计数器
	SysTick->CTRL |= 1<<0;   //使能系统滴答定时器
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器		
}


