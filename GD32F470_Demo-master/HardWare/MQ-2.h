#ifndef __MQ_2_H__
#define __MQ_2_H__

#include "stm32f10x.h"   // Device header
#include "math.h"
#include "Systic.h"
#include "time3.h"


#define CAL_PPM 20  // 校准环境中PPM值
#define RL			5		// RL阻值
static float R0; // 元件在洁净空气中的阻值

void ADC_MQ_2_Init(void);
uint16_t ADC_MQ_2_Trans(void);
uint16_t Get_Adc(uint8_t ch);


uint16_t Get_Adc_Average(uint8_t ch,uint8_t times);//两个入口参数，通道，取平均值的次数
void MQ2_cumlate(float RS);
float MQ2_GetPPM(void);


#endif

