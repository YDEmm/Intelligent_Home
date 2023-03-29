/*
 * @Author: JYM
 * @Date: 2023-01-20 20:08:40
 * @LastEditTime: 2023-01-24 17:22:10
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\User\Src\KEY.c
 * 
 */
#include "KEY.h"
#if 1

/**
 * @description: 按键初始化
 *               KEY1------PA0
 * @return {*}
 */
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能A口时钟
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;                //选择管脚0
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;        //输出速度为50MHZ
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;         //上拉
    //PA0
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}


/**
 * @description: 按键扫描函数
 * @return {*}
 */
u8 KEY_Scan(void)
{
    static u8 KEY_Value;

    if((KEY_Value == 1) && (KEY == 0))
    {
        KEY_Value = 0;
        BEEP_ON;
    }
    else if(KEY == 1)
    {
        KEY_Value = 1;
        BEEP_OFF;
    }
    return 0;
}

#elif 0

#endif


