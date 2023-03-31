#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
#include "main.h"


#define AUTOON			 0    //�Զ�ģʽ����
#define AUTOOFF 	     1    //�Զ�ģʽ�ر�
#define AUTOOFFHANDON 	 2    //�����ֶ�ģʽ
#define HANDNUM 		 3    //
#define THRESHOLD_SET 	 4    //��ֵ����
#define SETZIGBEE 		 5    //
#define ZIGBEECONFIG     6    //
#define SET              7    //
#define USELESS          8888


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
}ThresHod;

typedef struct{
    uint8_t data;
    int **Value_ThresHod;       //��ֵ��ʱֵ
    int **Control_ThresHod;     //״̬��ʱֵ
    int Flag;
    int USART5_Finish;
}IrDA_Type;

extern ThresHod Auto_Threshod;//��ֵ
extern IrDA_Type IrDA_Buff;

extern int Run_flag; //ȫ�ֱ��� ������Ļʹ��
void AUTO_Threshod_Init(void);  //��ֵ��ʼ��
void LED_Install(void);
void ZigBee_Send_Recevice(void);

int UART_Lcd_Task(ThresHod *Auto_Threshod);
int Change_mode(int Run_mode,int *Auto_flag);
void Get_Val(int Auto_flag,ThresHod Auto_Threshod);

void ESP8266_Send_Recevice(void);
void IrDA_Send_Recevice(ThresHod *Auto_Threshod,IrDA_Type *IrDA_Buff);


#ifdef __cplusplus
}
#endif

#endif /* __FUNCTION_H__ */
