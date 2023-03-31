/*
 * @Author: JYM
 * @Date: 2023-02-27 14:39:18
 * @LastEditTime: 2023-02-27 16:28:39
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--c8t6\HardWare\MultiTimer.c
 * 
 */
#include "MultiTimer.h"
#include <stdio.h>
#include "ZigBee_UART3.h"
#include "UART4_Lcd.h"


/* Timer handle list head. */
static MultiTimer* timerList = NULL;

/* Timer tick */
static PlatformTicksFunction_t platformTicksFunction = NULL;

int MultiTimerInstall(PlatformTicksFunction_t ticksFunc)
{
    platformTicksFunction = ticksFunc;
    return 0;
}


int MultiTimerStart(MultiTimer* timer, uint64_t timing, MultiTimerCallback_t callback, void* userData)
{
    if (!timer || !callback ) {
        return -1;
    }
    MultiTimer** nextTimer = &timerList;
    /* Remove the existing target timer. */
    for (; *nextTimer; nextTimer = &(*nextTimer)->next) {
        if (timer == *nextTimer) {
            *nextTimer = timer->next; /* remove from list */
            break;
        }
    }

    /* Init timer. */
    timer->deadline = platformTicksFunction() + timing;
    timer->callback = callback;
    timer->userData = userData;

    /* Insert timer. */
    for (nextTimer = &timerList;; nextTimer = &(*nextTimer)->next) {
        if (!*nextTimer) {
            timer->next = NULL;
            *nextTimer = timer;
            break;
        }
        if (timer->deadline < (*nextTimer)->deadline) {
            timer->next = *nextTimer;
            *nextTimer = timer;
            break;
        }
    }
    return 0;
}


int MultiTimerStop(MultiTimer* timer)
{
    MultiTimer** nextTimer = &timerList;
    /* Find and remove timer. */
    for (; *nextTimer; nextTimer = &(*nextTimer)->next) {
        MultiTimer* entry = *nextTimer;
        if (entry == timer) {
            *nextTimer = timer->next;
            break;
        }
    }
    return 0;
}

int MultiTimerYield(void)
{
    MultiTimer* entry = timerList;
    for (; entry; entry = entry->next) {
        /* Sorted list, just process with the front part. */
        if (platformTicksFunction() < entry->deadline) {
            return (int)(entry->deadline - platformTicksFunction());
        }
        /* remove expired timer from list */
        timerList = entry->next;

        /* call callback */
        if (entry->callback) {
            entry->callback(entry, entry->userData);
        }
    }
    return 0;
}

/*********************************用户代码***************************************/
uint64_t systick;

void systick_run(void)
{
    systick++;
}

uint64_t systick_get(void)
{
    return systick;
}
/* 创建一个定时器对象 */
MultiTimer tim1,tim2,tim3;
int Zibgbee_sendtime = 0;
/* 定义定时器超时回调函数 */
void timer1_callback(MultiTimer* timer, void* userData)
{
	//usb_printf("%s:%s\r\n",__FUNCTION__,(char *)userData);
//    if(Zigbee_Rec.Rec_Start == RECSTART)		//接收数据 开启计时
//		{			
//			Zigbee_Rec.Rec_Time++;					// 时间累加			
//			if(Zigbee_Rec.Rec_Time > Zigbee_Rec.Rec_Timeflag)
//			{
//				Zigbee_Rec.Rec_Finsh = RECEND;
//				Zigbee_Rec.Rec_Start = RECNOSTART;				
//				Zigbee_Rec.Rec_Time = 0; //清空时间
//				//后续处理字符串				
//			}					
//		}
//		if(Zigbee_Rec.Wait_Flag == 1)
//		{
//			Zigbee_Rec.Wait_Time++;
//		}
    Zibgbee_sendtime++;
    if(Lcd_Rec.Rec_Start == 1)        //收到第一帧数据 开始计时等待
    {
        Lcd_Rec.Rec_Time++;
        if(Lcd_Rec.Rec_Time > Lcd_Rec.Rec_Timeflag)
        {
            Lcd_Rec.Rec_Finsh = 1; //超时未收到消息 结束标志
        }
    }			
	MultiTimerStart(&tim1, 10, timer1_callback, "ZigBee_Recvice");
}
void timer2_callback(MultiTimer* timer, void* userData)
{
    ZigBee_Send_Flag = 1;   //标志位置1  zigbee两秒发送一次
	//usb_printf("%s:%s\r\n",__FUNCTION__,(char *)userData);
	MultiTimerStart(&tim2, 2000, timer2_callback, "ZigBee_printf");
}

//uint8_t times = 0;
//void timer3_callback(MultiTimer* timer, void* userData)
//{
//    times++;
//	//usb_printf("%s:%s\r\n",__FUNCTION__,(char *)userData);
//	MultiTimerStart(&tim3, 500, timer3_callback, "MQ_2");	
//}


