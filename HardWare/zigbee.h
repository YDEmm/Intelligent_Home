#ifndef __ZIGBEE_H
#define __ZIGBEE_H

#include "stm32f10x.h"
#include "io_bit(m3).h"
#include "time4.h"
#include "string.h"
//#include "hw_config.h"
#include "systic.h"
#include "string.h"
#include <stdio.h>
#define RX_BUF_MAX_LEN     1024         //最大接收缓存字节数

#define RECNOSTART	    0    //没开始
#define RECSTART 	    1    //开始接收
#define RECNOEND 		0    //没结束
#define RECEND 	        1    //接收结束


#define TRUE 1
#define FAIL 0

typedef struct Uart_rec
{
	int Rec_Start;						//开始接收标志
	int Rec_Finsh;						//结束接受标志
    
	int Rec_Num;						//接收字符数
	char Rec_String[RX_BUF_MAX_LEN];	//接收字符串
	
	int Rec_Time;						//超时时间
	int Rec_Timeflag;                   //理想超时时间
	
	int Wait_Flag;  //发送字符串标志
	int Wait_Time;  //发送字符串后等待回复标志
	
}Uart;


extern Uart Zigbee_Rec;
extern u8 ZigBee_Send_Flag;
extern u8 ZigBee_Recevice_Flag;
void Zigbee_Init(void);

void Zigbee_send(char *str);
char* Zigbee_recive(void);
int Zigbee_Read(int waittime,char recstr[]);

#endif
