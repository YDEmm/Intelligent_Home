#include "time4.h"


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

void TIM4_IRQHandler(void)
{		
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)              //�ж��Ƿ����ж�
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);					//�����־	 		
		if(Zigbee_Rec.Rec_Start == RECSTART)		//�������� ������ʱ
		{			
			Zigbee_Rec.Rec_Time++;					// ʱ���ۼ�			
			if(Zigbee_Rec.Rec_Time > Zigbee_Rec.Rec_Timeflag)
			{
				Zigbee_Rec.Rec_End = RECEND;
				Zigbee_Rec.Rec_Start = RECNOSTART;				
				printf("TIM4_IRQHandler %s\r\n",Zigbee_Rec.Rec_String);							
				Zigbee_Rec.Rec_Time = 0; //���ʱ��
				//���������ַ���				
			}					
		}
		if(Zigbee_Rec.Wait_Flag == 1)
		{
			Zigbee_Rec.Wait_Time++;
		}			
	}
}









