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
 * @description: DHT11测试函数
 * @return {*}
 */
void DHT11_Test(void)
{
    u8 Temperature = 0, Humidity = 0;//温湿度

    if(DHT11_Read_Data(&Temperature,NULL,&Humidity,NULL) != 0 )
    {
        printf("目前的温度为：%d℃\r\n",Temperature);
        printf("目前的湿度为：%d%%\r\n",Humidity);
    }
    
}

/**
 * @description: BH1750测试函数
 * @return {*}
 */
void BH1750_Test(void)
{
    printf("目前的光照强度为：%d\r\n",BH1750_Read_Data(SlaveAddress));
}


/**
 * @description: 烟雾传感器测试函数
 * @return {*}
 */
void MQ_2_Test(void)
{
    float Adc_MQ_2 = 0.0;
    Adc_MQ_2 = MQ2_GetPPM();

    printf("smoke:%.2f ppm\n", MQ2_GetPPM());//计算烟雾浓度并通过串口打印
    //printf("smoke:%d ppm\n", Get_Adc_Average(ADC_Channel_1,30));//输出模拟量   1---4096   0---0
}

/**
 * @description: 步进电机测试函数
 * @return {*}
 */
void Step_Motor_Test(void)
{
    /*
    函数第一个参数：控制正反转 1→正转   0→反转
    第二个参数：控制电机转的圈数
    第三个参数：延时*/
 
    Step_Motor_LOOP(1,1,1);  //步进电机正传
    delay_ms(500); 
    Step_Motor_LOOP(0,1,1);  //步进电机反传
    delay_ms(500);
//    Step_Motor_NUM(1,100,1); 
//    delay_ms(500);
}
