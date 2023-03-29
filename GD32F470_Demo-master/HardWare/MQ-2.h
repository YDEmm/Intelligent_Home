#ifndef __MQ_2_H__
#define __MQ_2_H__

#include "stm32f10x.h"   // Device header
#include "math.h"
#include "Systic.h"
#include "time3.h"


#define CAL_PPM 20  // У׼������PPMֵ
#define RL			5		// RL��ֵ
static float R0; // Ԫ���ڽྻ�����е���ֵ

void ADC_MQ_2_Init(void);
uint16_t ADC_MQ_2_Trans(void);
uint16_t Get_Adc(uint8_t ch);


uint16_t Get_Adc_Average(uint8_t ch,uint8_t times);//������ڲ�����ͨ����ȡƽ��ֵ�Ĵ���
void MQ2_cumlate(float RS);
float MQ2_GetPPM(void);


#endif

