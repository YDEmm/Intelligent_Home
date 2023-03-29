/*** 
 * @Author: JYM
 * @Date: 2023-01-25 21:15:57
 * @LastEditTime: 2023-01-25 21:53:37
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\User\Inc\function.h
 * @
 */
#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include "main.h"

typedef struct 
{
    int Temperature_Max;
    int Temperature_Min;
    
    int Humidity_Max;
    int Humidity_Min;
    
    int Intensity_Max;
    int Intensity_Min;
    
    int Smoke_Max;
    int Smoke_Min;
	
	int Air;
    int Curtain;
    int Window;
    int LED1;
    int LED2;

    int id;
}ThresHold;


extern ThresHold Auto_Threshod;          //阈值
extern int OLED_Mode_Flag;    //OLED模式标志位--0：开始界面-1：子节点信息-2：控制模式下状态
extern u8 OLED_Clear_Flag;
extern int Clear_Flag;
void ZigBee_Send_Recevice(void);
void LED_Test(void);

void Step_Motor_Test(void);

void USB_Status_Init(void);

void OLED_Task(void);

#endif

