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
 * MQ-2:    PA1->A0-----模拟量输出接口
 **************************************************/

int main()
{
    int y = 0,x=0;
    //TIM3_Init(5000, 7200);   //500ms 进一次中断
    TIM4_Int_Init(10, 7200);   //1ms 进一次中断
    Usart1_Init(115200);
    
    LED_Init();
    DHT11_Init();
    IIC_Init();
    BH1750_Init();
    ADC_MQ_2_Init();
    Step_Motor_Init();
    Relay_Init();
    OLED_Init();
    OLED_ColorTurn(0);//0正常显示，1 反色显示
    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
    
    Zigbee_Init();
    
    //Zigbee_send("111\r\n");
    
    /********************MultiTimer定时器**********************/
    MultiTimerInstall(systick_get);
    /* 启动定时器 */
	MultiTimerStart(&tim1, 10, timer1_callback, "ZigBee_Recvice");   //zigbee  10ms接收
	MultiTimerStart(&tim2, 20, timer2_callback, "ZigBee_Send");   //zigbee  20ms发送
	MultiTimerStart(&tim3, 500, timer3_callback, "MQ_2"); 
    /********************MultiTimer定时器**********************/
    while(1)
    {
#if 1   
        MultiTimerYield();//定时器
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


