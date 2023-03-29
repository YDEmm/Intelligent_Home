/*
 * @Author: JYM
 * @Date: 2023-01-26 09:26:09
 * @LastEditTime: 2023-01-26 09:44:13
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\HardWare\relay.c
 * 
 */
#include "relay.h"                

/**
 * @description: relay初始化
 *               DC+               5V
 *               DC-               GND
 *               IN                PA11
 * @return {*}
 */
void Relay_Init(void) 
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RELAY_CLK, ENABLE); //使能A口时钟
    
    GPIO_InitStruct.GPIO_Pin = RELAY1_PIN | RELAY2_PIN;                //选择管脚7
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;        //输出速度为50MHZ
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    //PA
    GPIO_Init(RELAY_PORT, &GPIO_InitStruct);
    
    RELAY1 = 0;  
    RELAY2 = 0;

}
/**
 * @description: relay
 * @return {*}
 */
void Relay_Loop(void) 
{
    GPIO_SetBits(RELAY_PORT, RELAY1_PIN);          //设置PA7为高电平
    GPIO_ResetBits(RELAY_PORT, RELAY2_PIN);          //设置PA6为高电平
    delay_ms(1000);//延时1秒
    GPIO_SetBits(RELAY_PORT, RELAY2_PIN);          //设置PA7为高电平
    GPIO_ResetBits(RELAY_PORT, RELAY1_PIN);          //设置PA7为高电平
    delay_ms(1000);
}


