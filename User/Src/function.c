/*
 * @Author: JYM
 * @Date: 2023-01-25 21:14:37
 * @LastEditTime: 2023-01-25 21:53:30
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\User\Src\function.c
 * 
 */
#include "function.h"

/**
 * @description: DHT11���Ժ���
 * @return {*}
 */
void DHT11_Test(void)
{
    u8 Temperature = 0, Humidity = 0;//��ʪ��

    if(DHT11_Read_Data(&Temperature,NULL,&Humidity,NULL) != 0 )
    {
        printf("Ŀǰ���¶�Ϊ��%d��\r\n",Temperature);
        printf("Ŀǰ��ʪ��Ϊ��%d%%\r\n",Humidity);
    }
    
}

/**
 * @description: BH1750���Ժ���
 * @return {*}
 */
void BH1750_Test(void)
{
    printf("Ŀǰ�Ĺ���ǿ��Ϊ��%d\r\n",BH1750_Read_Data(SlaveAddress));
}


/**
 * @description: �����������Ժ���
 * @return {*}
 */
void MQ_2_Test(void)
{
    float Adc_MQ_2 = 0.0;
    Adc_MQ_2 = MQ2_GetPPM();

    printf("smoke:%.2f ppm\n", MQ2_GetPPM());//��������Ũ�Ȳ�ͨ�����ڴ�ӡ
    //printf("smoke:%d ppm\n", Get_Adc_Average(ADC_Channel_1,30));//���ģ����   1---4096   0---0
}

/**
 * @description: ����������Ժ���
 * @return {*}
 */
void Step_Motor_Test(void)
{
    /*
    ������һ����������������ת 1����ת   0����ת
    �ڶ������������Ƶ��ת��Ȧ��
    ��������������ʱ*/
 
    Step_Motor_LOOP(1,1,1);  //�����������
    delay_ms(500); 
    Step_Motor_LOOP(0,1,1);  //�����������
    delay_ms(500);
//    Step_Motor_NUM(1,100,1); 
//    delay_ms(500);
}
