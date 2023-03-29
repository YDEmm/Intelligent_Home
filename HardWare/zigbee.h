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
#define RX_BUF_MAX_LEN     1024         //�����ջ����ֽ���

#define RECNOSTART	    0    //û��ʼ
#define RECSTART 	    1    //��ʼ����
#define RECNOEND 		0    //û����
#define RECEND 	        1    //���ս���


#define TRUE 1
#define FAIL 0

typedef struct Uart_rec
{
	int Rec_Start;						//��ʼ���ձ�־
	int Rec_Finsh;						//�������ܱ�־
    
	int Rec_Num;						//�����ַ���
	char Rec_String[RX_BUF_MAX_LEN];	//�����ַ���
	
	int Rec_Time;						//��ʱʱ��
	int Rec_Timeflag;                   //���볬ʱʱ��
	
	int Wait_Flag;  //�����ַ�����־
	int Wait_Time;  //�����ַ�����ȴ��ظ���־
	
}Uart;


extern Uart Zigbee_Rec;
extern u8 ZigBee_Send_Flag;
extern u8 ZigBee_Recevice_Flag;
void Zigbee_Init(void);

void Zigbee_send(char *str);
char* Zigbee_recive(void);
int Zigbee_Read(int waittime,char recstr[]);

#endif
