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


extern ThresHold Auto_Threshod;          //��ֵ
extern int OLED_Mode_Flag;    //OLEDģʽ��־λ--0����ʼ����-1���ӽڵ���Ϣ-2������ģʽ��״̬
extern u8 OLED_Clear_Flag;
extern int Clear_Flag;
void ZigBee_Send_Recevice(void);
void LED_Test(void);

void Step_Motor_Test(void);

void USB_Status_Init(void);

void OLED_Task(void);

#endif

