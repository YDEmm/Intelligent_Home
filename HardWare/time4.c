#include "time4.h"


void TIM4_Int_Init(u16 arr, u16 psc)

{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);         //开启 TIM4 的时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	                                                                  	
	/*  1:  函数  TIM_TimeBaseInit  初始化时基单元    */
	TIM_TimeBaseInitStrue.TIM_Period = arr;                     //自动装载值 入口参数传入 周期
	TIM_TimeBaseInitStrue.TIM_Prescaler = psc;                  //预分频系数 
	TIM_TimeBaseInitStrue.TIM_CounterMode = TIM_CounterMode_Up; //向上计数  、、模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStrue);
	
	/*  2:  函数 TIM_ARRPreloadConfig 使能ARR 上的预装载寄存器    */
	TIM_ARRPreloadConfig(TIM4, ENABLE); 
	/*  3:  函数 TIM_SelectOnePulseMode 脉冲模式设置 更新事件不停止   */
	TIM_SelectOnePulseMode(TIM4, TIM_OPMode_Repetitive); 
	/*  4:  函数 TIM_UpdateRequestConfig 生成重复脉冲 更新事件时计数器不停止   */
	TIM_UpdateRequestConfig(TIM4, TIM_UpdateSource_Global);	
	/*  5:  函数 TIM_UpdateDisableConfig   更新事件 使能更新事件  */
	TIM_UpdateDisableConfig(TIM4, DISABLE); 
	/*  6:  函数 TIM_GenerateEvent 定时器事件源 tim更新事件源  */
	TIM_GenerateEvent(TIM4, TIM_EventSource_Update);                    //生成手动更新事件 这里 UG位 置为 1 需要手动清除标志位 
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);                    		// 选择定时器号 取值为TIM1-TIM17 ，指定中断类型 TIM_IT_Update，TIM_IT_Trigger，使能
	  /****************    设置优先级  ****************************/
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;             		//TIM4 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   		//抢占优先级  数字小优先级高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          		//相应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            		 	//使能通道
	NVIC_Init(&NVIC_InitStructure);                             			//初始化
		
	TIM_Cmd(TIM4, ENABLE);                                       			//开启定时器
	
}

void TIM4_IRQHandler(void)
{		
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)              //判断是否发生中断
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);					//清除标志	 		
		if(Zigbee_Rec.Rec_Start == RECSTART)		//接收数据 开启计时
		{			
			Zigbee_Rec.Rec_Time++;					// 时间累加			
			if(Zigbee_Rec.Rec_Time > Zigbee_Rec.Rec_Timeflag)
			{
				Zigbee_Rec.Rec_End = RECEND;
				Zigbee_Rec.Rec_Start = RECNOSTART;				
				printf("TIM4_IRQHandler %s\r\n",Zigbee_Rec.Rec_String);							
				Zigbee_Rec.Rec_Time = 0; //清空时间
				//后续处理字符串				
			}					
		}
		if(Zigbee_Rec.Wait_Flag == 1)
		{
			Zigbee_Rec.Wait_Time++;
		}			
	}
}









