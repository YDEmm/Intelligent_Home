/*
 * @Author: JYM
 * @Date: 2023-01-24 22:36:29
 * @LastEditTime: 2023-01-24 23:08:20
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\HardWare\time3.c
 * 
 */
#include "time3.h"

///**
// * @description: TIM3初始化函数
// * @param {u16} arr
// * @param {u16} psc
// * 说明：
// * 时间（s） = 计数周期（arr） / 计数频率
// * 计数频率 = 时钟周期(hz) / 预分频系数(psc)
// * 1MHZ = 1000 000HZ
// * 计数频率 = 72000 000hz / psc
// * 1s = arr/计数频率
// * 计数频率= 72000000h/7200
// * 1s = arr/10000
// * psc = 7200
// * arr = 10000
// * @return {*}
// */
//void TIM3_Init(u16 arr,u16 psc)
//{
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitSture;
//    NVIC_InitTypeDef NVIC_InitStructure;


//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//定时器3，时钟
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);       //关闭PB4作为jntrst 使其成为普通管脚
//    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);		//tim3 功能部分映射

//    TIM_TimeBaseInitSture.TIM_Period = arr;//自动装载值
//    TIM_TimeBaseInitSture.TIM_Prescaler = psc;//预分频系数
//    TIM_TimeBaseInitSture.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
//    TIM_TimeBaseInitSture.TIM_ClockDivision = TIM_CKD_DIV1;

//    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitSture);//定时器3

//    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//定时器3，更新中断，使能

//    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

//    TIM_Cmd(TIM3,ENABLE);
//}
//uint8_t times = 0;
//void TIM3_IRQHandler(void)//中断函数，0.5秒中断一次
//{
//	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)//判断状态
//	 { 
//		 TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//清除中断待处理位
//		 times++;
//	 }
//}
	

	


