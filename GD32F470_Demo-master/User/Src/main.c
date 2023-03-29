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
    //TIM3_Init(5000, 7200);   //500ms 进一次中断
    TIM4_Int_Init(10, 7200);   //1ms 进一次中断
    //SysTick_Config(SystemCoreClock/1000);
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
    /********************USB虚拟串口**********************/
    OLED_Init();
    OLED_ShowString(10,1,"jym",3);
    OLED_ShowCHinese(10,5,2);   //根据字模的顺序
    
    //Zigbee_Init();
    
    //Zigbee_send("111\r\n");
    
    
    
    
    /********************MultiTimer定时器**********************/
    MultiTimerInstall(systick_get);
    /* 启动定时器 */
	MultiTimerStart(&tim1, 1000, timer1_callback, "111111111111111");
	MultiTimerStart(&tim2, 2000, timer2_callback, "222222222222222");
	MultiTimerStart(&tim3, 3000, timer3_callback, "333333333333333"); 
    /********************MultiTimer定时器**********************/
    while(1)
    {
#if 1   
        MultiTimerYield();//定时器
        //LED_Test();
        //DHT11_Test();
        //MQ_2_Test();
        //BH1750_Test();
        //usb_printf("111\r\n\r\n");   //插入换行
        //Step_Motor_Test();
		//Relay_Loop();
        
        
        
        
        
        
#endif
        
        
    }
        
}


