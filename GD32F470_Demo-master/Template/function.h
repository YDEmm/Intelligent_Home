#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
#include "main.h"


#define AUTOON			 0    //自动模式开启
#define AUTOOFF 	     1    //自动模式关闭
#define AUTOOFFHANDON 	 2    //设置手动模式
#define HANDNUM 		 3    //
#define THRESHOLD_SET 	 4    //阈值设置
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
    int **Value_ThresHod;       //阈值临时值
    int **Control_ThresHod;     //状态临时值
    int Flag;
    int USART5_Finish;
}IrDA_Type;

extern ThresHod Auto_Threshod;//阈值
extern IrDA_Type IrDA_Buff;

extern int Run_flag; //全局变量 操作屏幕使用
void AUTO_Threshod_Init(void);  //阈值初始化
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
