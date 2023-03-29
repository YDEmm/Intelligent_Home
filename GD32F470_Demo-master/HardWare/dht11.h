/*** 
 * @Author: JYM
 * @Date: 2023-01-21 13:54:52
 * @LastEditTime: 2023-01-24 17:25:28
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\HardWare\dht11.h
 * @
 */
#ifndef _dht11_H
#define _dht11_H
#include "io_bit(m3).h"
#include "systic.h"


#define DHT11 (GPIO_Pin_9) //PB9
#define GPIO_DHT11 GPIOB

#define DHT11_DQ_IN PBin(9)	  // ‰»Î
#define DHT11_DQ_OUT PBout(9)  // ‰≥ˆ

void DHT11_IO_OUT(void);
void DHT11_IO_IN(void);
uint8_t DHT11_Init(void);
void DHT11_Rst(void);
uint8_t DHT11_Check(void);
uint8_t DHT11_Read_Bit(void);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *temp_f,uint8_t *humi,uint8_t *humi_f)    ;

#endif
