/*** 
 * @Author: JYM
 * @Date: 2021-05-08 15:12:46
 * @LastEditTime: 2023-01-24 18:33:57
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\HardWare\gy30.h
 * @
 */
#ifndef __G3Y_H__
#define __G3Y_H__

#include "iic1.h"

/* 
����������IIC�����еĴӵ�ַ,����ģ��ADDRESS��ַ���Ų�ͬ�޸�
   ADDRESS���Žӵ�ʱ��ַΪ0x46���ӵ�Դʱ��ַΪ0xB8
*/
#define	  SlaveAddress   0x46


void BH1750_Init(void);
void BH1750_Write_COM(unsigned char Dev_Adder,unsigned char Common);
u16 BH1750_Read_Data(unsigned char Dev_Adder);

#endif

