/*
 * @Author: JYM
 * @Date: 2021-05-08 15:24:28
 * @LastEditTime: 2023-01-24 18:35:50
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\HardWare\iic1.c
 * 
 */
#include "iic1.h"
#include "Systic.h"

/**
 * @description: SDA��ʼ��
 * @return {*}
 */
void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	/*SDA-----PB11*/ //��������
	GPIO_InitStruct.GPIO_Pin = IIC_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_GPIO,&GPIO_InitStruct);
}

/**
 * @description: SDA��ʼ��
 * @return {*}
 */
void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	/*SDA-----PB11*/ //������� 50MHZ
	GPIO_InitStruct.GPIO_Pin = IIC_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_GPIO,&GPIO_InitStruct);
}

/**
 * @description: IIC1��ʼ��
 * 				 SDA-----PB11 //������� 50MHZ
 * 				 CLK-----PB10 //������� 50MHZ
 * @return {*}
 */
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��Bʱ��
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//����AFIOʱ��	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	/*SDA-----PB11*/ //������� 50MHZ
	GPIO_InitStruct.GPIO_Pin = IIC_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_GPIO,&GPIO_InitStruct);
	
	/*CLK-----PB10*/ //������� 50MHZ
	GPIO_InitStruct.GPIO_Pin = IIC_CLK;
	GPIO_Init(IIC_GPIO,&GPIO_InitStruct);
		
	GY30_CLK_H;
	GY30_SDA_H;
}

/**
 * @description: ��ʼ�ź�
 * @return {*}
 */
void IIC_Start(void)
{
	SDA_OUT();
	GY30_SDA_H;   							//���������߸ߵ�ƽ�������źţ�
	GY30_CLK_H;   							//����ʱ���߸ߵ�ƽ�������źţ�
	Delay_us(5);
	GY30_SDA_L;   							//���������������½���
	Delay_us(5);
	GY30_CLK_L;   							//����ʱ���������½���
}


/**
 * @description: �����ź�
 * @return {*}
 */
void IIC_Stop(void)
{
	SDA_OUT();					//���ģʽ
	GY30_CLK_L;							//����ʱ���ߵ͵�ƽ
	GY30_SDA_L;							//���������ߵ͵�ƽ
	GY30_CLK_H;							//����ʱ���߸ߵ�ƽ
	Delay_us(5);
	GY30_SDA_H;							//���������߸ߵ�ƽ  ������
	Delay_us(5);

}


/**
 * @description: ����Ӧ���ź�
 * @return {*}
 */
void IIC_Send_ACK(void)
{
	
	GY30_CLK_L;
	SDA_OUT();			   				//���ģʽ
	GY30_SDA_L;										//���������ߵ͵�ƽ
	Delay_us(5);
	GY30_CLK_H;										//����ʱ�����߸ߵ�ƽ    ����
	Delay_us(5);
	GY30_CLK_L;										//����ʱ�����ߵ͵�ƽ
}


/**
 * @description: ���ͷ�Ӧ���ź�
 * @return {*}
 */
void IIC_Send_NoACK(void)
{
	
	GY30_CLK_L;
	SDA_OUT(); 
	GY30_SDA_H;											//����������Ϊ�ߵ�ƽ
	Delay_us(5);
	GY30_CLK_H;
	Delay_us(5);
	GY30_CLK_L;
}

/**
 * @description: ����8λ����
 * @param {u8} dat
 * @return {*}
 */
void IIC_Send_Data(u8 dat)
{
	u8 i;
	SDA_OUT();
	GY30_CLK_L;
	for(i = 0; i < 8; i++)					//��8�δ�������  һλһλ����  ����
	{
	//	Delay_us(5);//SCL���ͺ�S��SDAҪ����ı䣬(�ȴ�ʱ�䲻�ܳ���0.9us)?????
		if(dat & 0x80)								//�ȷ����λ  1000 0000
		{
			GY30_SDA_H;     //д1
		}
		else
		{
			GY30_SDA_L;     //д0
		}
		dat <<= 1;									//���Ʋ���  �θ�λ-->���λ
		Delay_us(5);
		GY30_CLK_H;
		Delay_us(5);
		GY30_CLK_L;
		Delay_us(5);	
	}
}

/**
 * @description: �ȴ�Ӧ��
 * @return {*}
 */
u8 IIC_Get_ACK(void)
{
	u8 ERRTIME = 0;    //��ʱ����
	SDA_IN();
	//GY30_SDA_L;// û��Ӱ��
	Delay_us(5);
	GY30_CLK_H;
	Delay_us(5);
	while( READ_SDA() )
	{
		ERRTIME++;
		if(ERRTIME >=250)
		{
			IIC_Stop();
			return 1;
		}
	}
	GY30_CLK_L;
	return 0;
}


/**
 * @description: ��ȡ8λ����
 * @param {u8} ack
 * @return {*}
 */
u8 IIC_Read_Data(u8 ack)
{
	unsigned char i,date = 0;
	SDA_IN();
	for(i = 0; i < 8; i++)
	{
		
		GY30_CLK_L;
		Delay_us(5);
		GY30_CLK_H;
		date <<= 1;
		Delay_us(5);
		if( READ_SDA() )
		{
			date ++;
		}
		//Delay_us(2);
	}
	if(ack)
	{
		IIC_Send_ACK();																	//����Ӧ��
	}
	else
	{
		IIC_Send_NoACK();																	//���ͷ�Ӧ��
	}
	return date;

}


