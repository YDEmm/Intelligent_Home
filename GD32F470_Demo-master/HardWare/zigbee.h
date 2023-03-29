#ifndef __ZIGBEE_H
#define __ZIGBEE_H

#include "stm32f10x.h"
#include "io_bit(m3).h"
#include "time4.h"
#include "string.h"
#include "hw_config.h"
#include "systic.h"
#include "string.h"
#include <stdio.h>


#define NET  	PBin(0)
#define AT_HEX 	PBout(1)
#define RST 	PBout(7)

#define SETHEX() 	PBout(1)=0
#define SETAT() 	PBout(1)=1

#define NET_SUCCESS 0
#define NET_ERROR 1

#define RX_BUF_MAX_LEN     1024                                     //�����ջ����ֽ���

#define RECNOSTART	0    //���ս���
#define RECSTART 	1    //��ʼ����

#define RECNOEND 		0    //���ս���
#define RECEND 	1    //��ʼ����


#define TRUE 1
#define FAIL 0

typedef struct Uart_rec
{
	int Rec_Start;						//��ʼ���ձ�־
	int Rec_End;						//�������ܱ�־
	int Rec_Num;						//�����ַ���
	char Rec_String[RX_BUF_MAX_LEN];	//�����ַ���
	
	int Rec_Time;						//��ʱʱ��
	int Rec_Timeflag;                   //���볬ʱʱ��
	
	int Wait_Flag;  //�����ַ�����־
	int Wait_Time;  //�����ַ�����ȴ��ظ���־
	
}Uart;


extern Uart Zigbee_Rec;

void Zigbee_Init(void);
void Test(void);

int CC2530_DEV (char mode);
int CC2530_Mode (char mode);
int CC2530_GROUP ( void );
int CC2530_CH ( void );
int CC2530_PANID ( void );
int CC2530_RESET ( void );
int CC2530_IN_AT ( void );
int CC2530_EXIT_AT ( void );
void Zigbee_send(char *str);
char* Zigbee_recive(void);

#endif
