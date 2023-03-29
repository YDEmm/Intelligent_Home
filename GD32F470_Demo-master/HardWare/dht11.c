/*
 * @Author: JYM
 * @Date: 2023-01-21 13:56:13
 * @LastEditTime: 2023-01-24 17:26:18
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\HardWare\dht11.c
 * 
 */
#include "dht11.h"

/**
 * @description: DHT11��ʼ��
 * 				 DHT11------PB9
 * @return {*}����0����ʼ���ɹ���1��ʧ��
 */
uint8_t DHT11_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_9);	   //����

	DHT11_Rst();	  
	return DHT11_Check();	
}

/**
 * @description: ��λDHT11
 * @return {*}
 */
void DHT11_Rst()	   
{                 
    DHT11_DQ_OUT=0; 		//����DQ
    delay_ms(20);    	//��������18ms
    DHT11_DQ_OUT=1; 		//DQ=1 
	delay_us(30);     //��������20~40us
}

/**
 * @description: �ȴ�DHT11�Ļ�Ӧ
 * @return {*}����1:δ��⵽DHT11�Ĵ��ڣ�����0:����
 */
uint8_t DHT11_Check() 	   
{   
	uint8_t retry=0;
    while (DHT11_DQ_IN&&retry<80)//DHT11������40~50us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=80)return 1;
	else retry=0;
	
    while (!DHT11_DQ_IN&&retry<100)//DHT11���ͺ���ٴ�����40~50us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

/**
 * @description: ��DHT11��ȡһ��λ
 * @return {*}����ֵ��1/0
 */
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(DHT11_DQ_IN&&retry<100)//�ȴ���Ϊ�͵�ƽ 12-14us ��ʼ
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//�ȴ���ߵ�ƽ	 26-28us��ʾ0,116-118us��ʾ1
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//�ȴ�40us
	
	
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

/**
 * @description: ��DHT11��ȡһ���ֽ�
 * @return {*}����ֵ������������
 */
uint8_t DHT11_Read_Byte(void)    
{        
    uint8_t i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}

/**
 * @description: ��DHT11��ȡһ������
 * @param {uint8_t} *temp  ��temp:�¶�ֵ(��Χ:0~50��)
 * @param {uint8_t} *temp_f  ��
 * @param {uint8_t} *humi  ��humi:ʪ��ֵ(��Χ:20%~90%)
 * @param {uint8_t} *humi_f  ��
 * @return {*}����ֵ��0,����;1,��ȡʧ��g
 */
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *temp_f,uint8_t *humi,uint8_t *humi_f)    
{        
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];			
			*temp=buf[2];
		}	
		
	}
    else return 1;
	return 0;	    
}



