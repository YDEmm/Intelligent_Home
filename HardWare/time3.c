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
// * @description: TIM3��ʼ������
// * @param {u16} arr
// * @param {u16} psc
// * ˵����
// * ʱ�䣨s�� = �������ڣ�arr�� / ����Ƶ��
// * ����Ƶ�� = ʱ������(hz) / Ԥ��Ƶϵ��(psc)
// * 1MHZ = 1000 000HZ
// * ����Ƶ�� = 72000 000hz / psc
// * 1s = arr/����Ƶ��
// * ����Ƶ��= 72000000h/7200
// * 1s = arr/10000
// * psc = 7200
// * arr = 10000
// * @return {*}
// */
//void TIM3_Init(u16 arr,u16 psc)
//{
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitSture;
//    NVIC_InitTypeDef NVIC_InitStructure;


//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//��ʱ��3��ʱ��
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);       //�ر�PB4��Ϊjntrst ʹ���Ϊ��ͨ�ܽ�
//    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);		//tim3 ���ܲ���ӳ��

//    TIM_TimeBaseInitSture.TIM_Period = arr;//�Զ�װ��ֵ
//    TIM_TimeBaseInitSture.TIM_Prescaler = psc;//Ԥ��Ƶϵ��
//    TIM_TimeBaseInitSture.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
//    TIM_TimeBaseInitSture.TIM_ClockDivision = TIM_CKD_DIV1;

//    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitSture);//��ʱ��3

//    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//��ʱ��3�������жϣ�ʹ��

//    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//    NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

//    TIM_Cmd(TIM3,ENABLE);
//}
//uint8_t times = 0;
//void TIM3_IRQHandler(void)//�жϺ�����0.5���ж�һ��
//{
//	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)//�ж�״̬
//	 { 
//		 TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//����жϴ�����λ
//		 times++;
//	 }
//}
	

	


