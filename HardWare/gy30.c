/*
 * @Author: JYM
 * @Date: 2021-05-08 15:13:04
 * @LastEditTime: 2023-01-24 18:33:01
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\HardWare\gy30.c
 * 
 */
#include "gy30.h"
#include "Systic.h"
#include "iic1.h"


/**
 * @description: BH1750дָ���
 * @param {unsigned char} Dev_Adder   ģ��IICͨѶ��ַ
 * @param {unsigned char} Common      ��Ҫ���͵Ŀ���ָ��
 * @return {*}
 */
void BH1750_Write_COM(unsigned char Dev_Adder,unsigned char Common)
{
	IIC_Start();                   //��ʼ�ź�
	IIC_Send_Data(Dev_Adder);      //���ʹ��豸��ַ�����豸��ַ+��д����λ��
	if( IIC_Get_ACK() )            //���Ӧ��
	{
		goto WR_End;           
	} 
	IIC_Send_Data(Common);       	 //���Ͳ���ָ��
	if( IIC_Get_ACK() )            //���Ӧ��
	{
		goto WR_End;
	}
WR_End:	
	IIC_Stop();	                   //���ͽ����ź�
}

/**
 * @description: BH1750�����ݺ���
 * @param {unsigned char} Dev_Adder   ģ��IICͨѶ��ַ
 * @return {*}
 */
u16 BH1750_Read_Data(unsigned char Dev_Adder)
{
	unsigned char Buff[2] = {0}; 
	u16 Data = 0;
	IIC_Start();                           //��ʼ�ź�
	IIC_Send_Data(Dev_Adder | 0x01);       //���ʹ��豸��ַ�����豸��ַ+��д����λ��
	if( IIC_Get_ACK() )                    //���Ӧ��
	{
		goto WR_End;           
	} 
	/* ��ȡ�������� */
	Buff[0]= IIC_Read_Data(1);             //��ȡ���ֽڹ������� ������Ӧ���ź�               
	Buff[1] = IIC_Read_Data(0);            //��ȡ���ֽڹ������� �����ͷ�Ӧ���ź�                
	
WR_End:	
	IIC_Stop();	                          //���ͽ����ź�
	Delay_us(5);
	
    /* �ϲ���ǿ���� */	
	Data = Buff[0]; 
	Data = (Data << 8) | Buff[1];         //�ϳ����ݣ�����������
	
	return Data;													//����ǿ����ֵ
}

/**
 * @description: BH1750��ʼ������
 * @return {*}������Ҫ�ο�pdf���в����޸�
 */
void BH1750_Init(void)
{
	BH1750_Write_COM(SlaveAddress,0x01);      //����Դ
	BH1750_Write_COM(SlaveAddress,0x10);      //����ģʽ
	Delay_us(180);                            //�ȵ���������
}



