/*
 * @Author: JYM
 * @Date: 2023-01-25 15:49:03
 * @LastEditTime: 2023-01-25 21:51:03
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\HardWare\step_motor.c
 * 
 */
#include "step_motor.h"

uint8_t STEP; 

/**
 * @description: ��������ӿڳ�ʼ��
                 PB3  PB4   PB5   PB12
 * @return {*}
 */
void Step_Motor_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(STEP_MOTOR_CLK, ENABLE);
 
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStruct.GPIO_Pin = STEP_MOTOR_A;
    GPIO_Init(STEP_MOTOR_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin = STEP_MOTOR_B;
    GPIO_Init(STEP_MOTOR_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin = STEP_MOTOR_C;
    GPIO_Init(STEP_MOTOR_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin = STEP_MOTOR_D;
    GPIO_Init(STEP_MOTOR_PORT, &GPIO_InitStruct);
 
    GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_A);
    GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_B);
    GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_C);
    GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_D);	
}

/**
 * @description: ����ϵ� ȫ����
 * @return {*}
 */
void Step_Motor_OFF(void) 
{
    GPIO_ResetBits(STEP_MOTOR_PORT, (STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D));//���ӿ���0
}
 
/**
 * @description: �������8��
 * @param {uint8_t} a
 * @param {uint32_t} speed
 * @return {*}
 */
void Step_Motor_8A(uint8_t a,uint32_t speed)  
{
	switch(a)
    {
		case 0:
            GPIO_ResetBits(STEP_MOTOR_PORT,(STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D));//0
            GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A);//1
			break;
		case 1:
            GPIO_ResetBits(STEP_MOTOR_PORT,(STEP_MOTOR_C | STEP_MOTOR_D));//0
            GPIO_SetBits(STEP_MOTOR_PORT,(STEP_MOTOR_A | STEP_MOTOR_B));//1
			break;
		case 2:
            GPIO_ResetBits(STEP_MOTOR_PORT,(STEP_MOTOR_A | STEP_MOTOR_C | STEP_MOTOR_D));//0
            GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B);//1
			break;
		case 3:
            GPIO_ResetBits(STEP_MOTOR_PORT,(STEP_MOTOR_A | STEP_MOTOR_D));//0
            GPIO_SetBits(STEP_MOTOR_PORT,(STEP_MOTOR_B | STEP_MOTOR_C));//1
			break;
		case 4:
            GPIO_ResetBits(STEP_MOTOR_PORT,(STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_D));//0
            GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C);//1
			break;
		case 5:
            GPIO_ResetBits(STEP_MOTOR_PORT,(STEP_MOTOR_A | STEP_MOTOR_B));//0
            GPIO_SetBits(STEP_MOTOR_PORT,(STEP_MOTOR_C | STEP_MOTOR_D));//1
			break;
		case 6:
            GPIO_ResetBits(STEP_MOTOR_PORT,(STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C));//0
            GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_D);//1
			break;
		case 7:
            GPIO_ResetBits(STEP_MOTOR_PORT,(STEP_MOTOR_B | STEP_MOTOR_C));//0
            GPIO_SetBits(STEP_MOTOR_PORT,(STEP_MOTOR_A | STEP_MOTOR_D));//1
			break;
		default:
			break;
	}
	delay_ms(speed); //��ʱ
	Step_Motor_OFF();//����ϵ�״̬�����������
}

/**
 * @description: �������������
 * @param {uint8_t} rl
 * @param {uint16_t} num
 * @param {uint32_t} speed
 * @return {*}
 */
void Step_Motor_NUM(uint8_t rl,uint16_t num,uint32_t speed)  
{
	u16 i;
	for(i = 0; i < num; i++)
    {	
		if(rl == 1)                     //��rl=1��ת��RL=0��ת
        { 
			STEP++;
			if(STEP > 7)  STEP = 0;
		}
        else
        {
			if(STEP == 0) STEP = 8;
			STEP--;
		}
		Step_Motor_8A(STEP,speed);
	}
}
 
/**
 * @description: �����Ȧ������
 * @param {uint8_t} rl
 * @param {uint8_t} LOOP
 * @param {uint32_t} speed
 * @return {*}
 */
void Step_Motor_LOOP(uint8_t rl,uint8_t LOOP,uint32_t speed)  
{
	Step_Motor_NUM(rl,(LOOP * 4076), speed);
}





