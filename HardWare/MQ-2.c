/*
 * @Author: JYM
 * @Date: 2023-01-24 21:08:31
 * @LastEditTime: 2023-01-24 21:29:26
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\HardWare\MQ-2.c
 * 
 */
#include "MQ-2.h"

/**
 * @description: GPIO��ʼ��	PA0 ADC1_IN0 ģ������
 * @return {*}
 */
void ADC_MQ_2_Init(void)
{
    GPIO_InitTypeDef GPIO_Initstructre;     
    ADC_InitTypeDef ADC_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1,ENABLE);//ʹ��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��io��
    GPIO_Initstructre.GPIO_Mode=GPIO_Mode_AIN; 
    GPIO_Initstructre.GPIO_Pin=GPIO_Pin_1;
    GPIO_Initstructre.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_Initstructre);
    GPIO_SetBits(GPIOA,GPIO_Pin_1);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);//��֤������14M
    ADC_DeInit(ADC1);//��λADC1
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//��ʹ������ɨ��
    ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;//�����Ҷ���
    ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//�������
    ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;//����ģʽ
    ADC_InitStruct.ADC_NbrOfChannel = 1;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;//��ʹ��ɨ��ģʽ
    ADC_Init(ADC1,&ADC_InitStruct);
    ADC_Cmd(ADC1,ENABLE);//ʹ��ָ����ADC1
    ADC_ResetCalibration(ADC1);//ʹ�ܸ�λУ׼  

    while(ADC_GetResetCalibrationStatus(ADC1));//�ȴ���λУ׼����

    ADC_StartCalibration(ADC1);//����ADУ׼

    while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}


/**
 * @description: ADC��ȡ����
 * @return {*}����50�ε�ƽ��ֵ
 */
uint16_t ADC_MQ_2_Trans(void)
{
	uint16_t adc_value = 0;
	uint8_t i = 0;
	
	for(i = 0; i < 50; i++)
	{ 
		//��ʼת��
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);

		//ת���Ƿ����
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) != SET);

		adc_value = adc_value + ADC_GetConversionValue(ADC1);//��ADC�е�ֵ
	}
	
	return adc_value / 50;
}


uint16_t Get_Adc(uint8_t ch)
{
    ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);//ADC1��ͨ��1�����òɼ�����

    ADC_SoftwareStartConvCmd(ADC1,ENABLE);//�����λ

    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//״̬�����ı�

    return ADC_GetConversionValue(ADC1);//����ֵ	

}

uint16_t Get_Adc_Average(uint8_t ch,uint8_t times)//������ڲ�����ͨ����ȡƽ��ֵ�Ĵ���
{
    uint32_t temp_val = 0;
    uint8_t t;
    
    for(t = 0; t < times; t++)
    {
        temp_val += Get_Adc(ch);
        delay_ms(5);
    }
        return temp_val/times;

}

/********************************************
 * 1.651428	          200               *
 * 1.437143	          300               *
 * 1.257143	          400               *
 * 1.137143	          500               *
 * 1		          600               *
 * 0.928704	          700               *
 * 0.871296	          800               *
 * 0.816667	          900               *
 * 0.785714	          1000              *
 * 0.574393	          2000              *
 * 0.466047	          3000              *
 * 0.415581	          4000              *
 * 0.370478	          5000              *
 * 0.337031	          6000              *
 * 0.305119	          7000              *
 * 0.288169	          8000              *
 * 0.272727	          9000              *
 * 0.254795	          10000             *
 *                                      *
 * ppm = 613.9f * pow(RS/RL, -2.074f)   *
 ***************************************/
void MQ2_cumlate(float RS)
{
		R0 = RS / pow(CAL_PPM / 613.9f, 1 / -2.074f);
}

float MQ2_GetPPM(void)
{   
	  u16 adcx;
	  adcx=Get_Adc_Average(ADC_Channel_1,30);//ADC1,ȡ30�ε�ƽ��ֵ
      float Vrl = 3.3f * adcx / 4096.f;//3.3v�Ĳο���ѹ��4096��
	  Vrl = ( (float)( (int)( (Vrl+0.005)*100 ) ) )/100;
      float RS = (3.3f - Vrl) / Vrl * RL;
	  
      if(times < 6) // ��ȡϵͳִ��ʱ�䣬3sǰ����У׼���õ��˶�ʱ��
       {
		  R0 = RS / pow(CAL_PPM / 613.9f, 1 / -2.074f);//У׼R0
       } 
	  float ppm = 613.9f * pow(RS/R0, -2.074f);

      return  ppm;
}





