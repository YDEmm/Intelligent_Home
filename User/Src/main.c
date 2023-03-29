/*
 * @Author: JYM
 * @Date: 2023-01-20 20:08:40
 * @LastEditTime: 2023-01-24 23:09:55
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\User\Src\main.c
 * 
 */
#include "main.h"
#include "function.h"
/***************************************************
 * USART1:PA9 PA10   USART2:PA2 PA3
 * LED1: PA5 L
 * BEEP:PA6 H
 * DHT11:PB9 H
 * GY30:   PB10->SCL SDA->PB11 ADDR->GND
 * MQ-2:    PA1->A0-----ģ��������ӿ�
 **************************************************/

int main()
{
    int y = 0,x=0;
    //TIM3_Init(5000, 7200);   //500ms ��һ���ж�
    TIM4_Int_Init(10, 7200);   //1ms ��һ���ж�
    Usart1_Init(115200);
    
    LED_Init();
    DHT11_Init();
    IIC_Init();
    BH1750_Init();
    ADC_MQ_2_Init();
    Step_Motor_Init();
    Relay_Init();
    OLED_Init();
    OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
    OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
    
    Zigbee_Init();
    
    //Zigbee_send("111\r\n");
    
    /********************MultiTimer��ʱ��**********************/
    MultiTimerInstall(systick_get);
    /* ������ʱ�� */
	MultiTimerStart(&tim1, 10, timer1_callback, "ZigBee_Recvice");   //zigbee  10ms����
	MultiTimerStart(&tim2, 20, timer2_callback, "ZigBee_Send");   //zigbee  20ms����
	MultiTimerStart(&tim3, 500, timer3_callback, "MQ_2"); 
    /********************MultiTimer��ʱ��**********************/
    while(1)
    {
#if 1   
        MultiTimerYield();//��ʱ��
        ZigBee_Send_Recevice();
        
        if(Clear_Flag != OLED_Mode_Flag)
        {
            Clear_Flag = OLED_Mode_Flag;
            OLED_Clear();
        }
        OLED_Task();
        //LED_Test();
        
        //Step_Motor_Test();
		//Relay_Loop();
        


          
#endif    
    }     
}


