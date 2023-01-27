/*** 
 * @Author: JYM
 * @Date: 2023-01-25 15:49:11
 * @LastEditTime: 2023-01-25 16:07:28
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\HardWare\step_motor.h
 * @
 */
#ifndef __STEP_MOTOR_H__
#define __STEP_MOTOR_H__

#include "stm32f10x.h"                   // Device header
#include "sys.h"
#include "Systic.h"
 
extern uint8_t STEP;                    //定义单步计数 全局变量
 
#define STEP_MOTOR_PORT			    (GPIOB)	    //定义IO接口所在组
#define STEP_MOTOR_A				(GPIO_Pin_3)	//定义IO接口
#define STEP_MOTOR_B				(GPIO_Pin_4)	//定义IO接口
#define STEP_MOTOR_C				(GPIO_Pin_5)	//定义IO接口
#define STEP_MOTOR_D				(GPIO_Pin_6)	//定义IO接口
#define STEP_MOTOR_CLK              (RCC_APB2Periph_GPIOB)


void Step_Motor_Init(void);                             //初始化
void Step_Motor_OFF(void);                              //断电状态
void Step_Motor_8A(uint8_t a, uint32_t speed);
void Step_Motor_NUM(uint8_t rl,uint16_t num,uint32_t speed);  //电机按步数运行
void Step_Motor_LOOP(uint8_t rl,uint8_t LOOP,uint32_t speed);  //电机按圈数运行

#endif

