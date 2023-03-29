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
 * @description: DHT11初始化
 * 				 DHT11------PB9
 * @return {*}返回0：初始化成功，1：失败
 */
uint8_t DHT11_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_9);	   //拉高

	DHT11_Rst();	  
	return DHT11_Check();	
}

/**
 * @description: 复位DHT11
 * @return {*}
 */
void DHT11_Rst()	   
{                 
    DHT11_DQ_OUT=0; 		//拉低DQ
    delay_ms(20);    	//拉低至少18ms
    DHT11_DQ_OUT=1; 		//DQ=1 
	delay_us(30);     //主机拉高20~40us
}

/**
 * @description: 等待DHT11的回应
 * @return {*}返回1:未检测到DHT11的存在；返回0:存在
 */
uint8_t DHT11_Check() 	   
{   
	uint8_t retry=0;
    while (DHT11_DQ_IN&&retry<80)//DHT11会拉低40~50us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=80)return 1;
	else retry=0;
	
    while (!DHT11_DQ_IN&&retry<100)//DHT11拉低后会再次拉高40~50us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

/**
 * @description: 从DHT11读取一个位
 * @return {*}返回值：1/0
 */
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(DHT11_DQ_IN&&retry<100)//等待变为低电平 12-14us 开始
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//等待变高电平	 26-28us表示0,116-118us表示1
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//等待40us
	
	
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

/**
 * @description: 从DHT11读取一个字节
 * @return {*}返回值：读到的数据
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
 * @description: 从DHT11读取一次数据
 * @param {uint8_t} *temp  ；temp:温度值(范围:0~50°)
 * @param {uint8_t} *temp_f  ；
 * @param {uint8_t} *humi  ；humi:湿度值(范围:20%~90%)
 * @param {uint8_t} *humi_f  ；
 * @return {*}返回值：0,正常;1,读取失败g
 */
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *temp_f,uint8_t *humi,uint8_t *humi_f)    
{        
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
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



