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
 * @description: GPIO初始化	PA0 ADC1_IN0 模拟输入
 * @return {*}
 */
void ADC_MQ_2_Init(void)
{
    GPIO_InitTypeDef GPIO_Initstructre;     
    ADC_InitTypeDef ADC_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1,ENABLE);//使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能io口
    GPIO_Initstructre.GPIO_Mode=GPIO_Mode_AIN; 
    GPIO_Initstructre.GPIO_Pin=GPIO_Pin_1;
    GPIO_Initstructre.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_Initstructre);
    GPIO_SetBits(GPIOA,GPIO_Pin_1);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);//保证不超过14M
    ADC_DeInit(ADC1);//复位ADC1
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//不使能连续扫描
    ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;//数据右对齐
    ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//软件触发
    ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;//独立模式
    ADC_InitStruct.ADC_NbrOfChannel = 1;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;//不使用扫描模式
    ADC_Init(ADC1,&ADC_InitStruct);
    ADC_Cmd(ADC1,ENABLE);//使能指定的ADC1
    ADC_ResetCalibration(ADC1);//使能复位校准  

    while(ADC_GetResetCalibrationStatus(ADC1));//等待复位校准结束

    ADC_StartCalibration(ADC1);//开启AD校准

    while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
}


/**
 * @description: ADC读取数据
 * @return {*}采样50次的平均值
 */
uint16_t ADC_MQ_2_Trans(void)
{
	uint16_t adc_value = 0;
	uint8_t i = 0;
	
	for(i = 0; i < 50; i++)
	{ 
		//开始转换
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);

		//转换是否结束
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) != SET);

		adc_value = adc_value + ADC_GetConversionValue(ADC1);//读ADC中的值
	}
	
	return adc_value / 50;
}


uint16_t Get_Adc(uint8_t ch)
{
    ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);//ADC1，通道1，配置采集周期

    ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件复位

    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//状态发生改变

    return ADC_GetConversionValue(ADC1);//返回值	

}

uint16_t Get_Adc_Average(uint8_t ch,uint8_t times)//两个入口参数，通道，取平均值的次数
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
	  adcx=Get_Adc_Average(ADC_Channel_1,30);//ADC1,取30次的平均值
      float Vrl = 3.3f * adcx / 4096.f;//3.3v的参考电压，4096份
	  Vrl = ( (float)( (int)( (Vrl+0.005)*100 ) ) )/100;
      float RS = (3.3f - Vrl) / Vrl * RL;
	  
      if(times < 6) // 获取系统执行时间，3s前进行校准，用到了定时器
       {
		  R0 = RS / pow(CAL_PPM / 613.9f, 1 / -2.074f);//校准R0
       } 
	  float ppm = 613.9f * pow(RS/R0, -2.074f);

      return  ppm;
}





