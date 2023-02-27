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
    //TIM3_Init(5000, 7200);   //500ms ��һ���ж�
    TIM4_Int_Init(10, 7200);   //1ms ��һ���ж�
    //SysTick_Config(SystemCoreClock/1000);
    Usart1_Init(115200);
    
    LED_Init();
    DHT11_Init();
    IIC_Init();
    BH1750_Init();
    ADC_MQ_2_Init();
    Step_Motor_Init();
    Relay_Init();
    /********************USB���⴮��**********************/
    delay_ms(1800);
    USB_Port_Set(0); 	//USB�ȶϿ�
	delay_ms(700);
	USB_Port_Set(1);	//USB�ٴ�����
 	Set_USBClock();   
 	USB_Interrupts_Config();    
 	USB_Init();	  
    /********************USB���⴮��**********************/
    OLED_Init();
    OLED_ShowString(10,1,"jym",3);
    OLED_ShowCHinese(10,5,2);   //������ģ��˳��
    
    //Zigbee_Init();
    
    //Zigbee_send("111\r\n");
    
    
    
    
    /********************MultiTimer��ʱ��**********************/
    MultiTimerInstall(systick_get);
    /* ������ʱ�� */
	MultiTimerStart(&tim1, 1000, timer1_callback, "111111111111111");
	MultiTimerStart(&tim2, 2000, timer2_callback, "222222222222222");
	MultiTimerStart(&tim3, 3000, timer3_callback, "333333333333333"); 
    /********************MultiTimer��ʱ��**********************/
    while(1)
    {
#if 1   
        MultiTimerYield();//��ʱ��
        //LED_Test();
        //DHT11_Test();
        //MQ_2_Test();
        //BH1750_Test();
        //usb_printf("111\r\n\r\n");   //���뻻��
        //Step_Motor_Test();
		//Relay_Loop();
        
        
        
        
        
        
#endif
        
        
    }
        
}


