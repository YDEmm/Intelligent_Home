/*
 * @Author: JYM
 * @Date: 2023-02-11 09:16:05
 * @LastEditTime: 2023-02-27 16:41:38
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--c8t6\HardWare\time4.c
 * 
 */
#include "time4.h"


/**
 * @description: TIM4_Int_Init
 * @param {u16} arr
 * @param {u16} psc
 * @return {*}
 * ˵����
 * ʱ�䣨s�� = �������ڣ�arr�� / ����Ƶ��
 * ����Ƶ�� = ʱ������(hz) / Ԥ��Ƶϵ��(psc)
 * 1MHZ = 1000 000HZ
 * ����Ƶ�� = 72000 000hz / psc
 * 1s = arr/����Ƶ��
 * ����Ƶ��= 72000000h/7200
 * 1s = arr/10000
 * psc = 7200
 * arr = 10000
 */
void TIM4_Int_Init(u16 arr, u16 psc)

{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);         //���� TIM4 ��ʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	                                                                  	
	/*  1:  ����  TIM_TimeBaseInit  ��ʼ��ʱ����Ԫ    */
	TIM_TimeBaseInitStrue.TIM_Period = arr;                     //�Զ�װ��ֵ ��ڲ������� ����
	TIM_TimeBaseInitStrue.TIM_Prescaler = psc;                  //Ԥ��Ƶϵ�� 
	TIM_TimeBaseInitStrue.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���  ����ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStrue);
	
	/*  2:  ���� TIM_ARRPreloadConfig ʹ��ARR �ϵ�Ԥװ�ؼĴ���    */
	TIM_ARRPreloadConfig(TIM4, ENABLE); 
	/*  3:  ���� TIM_SelectOnePulseMode ����ģʽ���� �����¼���ֹͣ   */
	TIM_SelectOnePulseMode(TIM4, TIM_OPMode_Repetitive); 
	/*  4:  ���� TIM_UpdateRequestConfig �����ظ����� �����¼�ʱ��������ֹͣ   */
	TIM_UpdateRequestConfig(TIM4, TIM_UpdateSource_Global);	
	/*  5:  ���� TIM_UpdateDisableConfig   �����¼� ʹ�ܸ����¼�  */
	TIM_UpdateDisableConfig(TIM4, DISABLE); 
	/*  6:  ���� TIM_GenerateEvent ��ʱ���¼�Դ tim�����¼�Դ  */
	TIM_GenerateEvent(TIM4, TIM_EventSource_Update);                    //�����ֶ������¼� ���� UGλ ��Ϊ 1 ��Ҫ�ֶ������־λ 
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);                    		// ѡ��ʱ���� ȡֵΪTIM1-TIM17 ��ָ���ж����� TIM_IT_Update��TIM_IT_Trigger��ʹ��
	  /****************    �������ȼ�  ****************************/
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;             		//TIM4 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   		//��ռ���ȼ�  ����С���ȼ���
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          		//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            		 	//ʹ��ͨ��
	NVIC_Init(&NVIC_InitStructure);                             			//��ʼ��
		
	TIM_Cmd(TIM4, ENABLE);                                       			//������ʱ��
	
}

extern void systick_run(void);
void TIM4_IRQHandler(void)
{		
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)              //�ж��Ƿ����ж�
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);					//�����־	 		
        /*********MultiTimer��ʱ��**********/
        systick_run();
	}
}









