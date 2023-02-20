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
    TIM3_Init(5000, 7200);   //0.5s 进一次中断
    TIM4_Int_Init(99, 7199);
    Usart1_Init(115200);
    LED_Init();
    DHT11_Init();
    IIC_Init();
    BH1750_Init();
    ADC_MQ_2_Init();
    Step_Motor_Init();
    Relay_Init();
    /********************USB虚拟串口**********************/
    delay_ms(1800);
    USB_Port_Set(0); 	//USB先断开
	delay_ms(700);
	USB_Port_Set(1);	//USB再次连接
 	Set_USBClock();   
 	USB_Interrupts_Config();    
 	USB_Init();	  

    OLED_Init();
    OLED_ShowString(10,1,"jym",3);
    OLED_ShowCHinese(10,5,2);   //根据字模的顺序
    
    //Zigbee_Init();
    
    //Zigbee_send("111\r\n");
    
    
    while(1)
    {
#if 1   
        //LED_Test();
        usb_printf("111\r\n\r\n");   //插入换行
        //Step_Motor_Test();
		//Relay_Loop();
        
        
        
        
        
#endif
        
        
    }
        
}


