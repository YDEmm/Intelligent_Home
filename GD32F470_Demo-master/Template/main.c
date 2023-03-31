#include "gd32f4xx.h"
#include <stdio.h>
#include "main.h"
#include "function.h"


/*!
    \brief    toggle the led every 500ms
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_spark(void)
{
    static __IO uint32_t timingdelaylocal = 0U;

    if(timingdelaylocal) {

        if(timingdelaylocal < 500U) {
            gd_eval_led_on(LED1);
        } else {
            gd_eval_led_off(LED1);
        }

        timingdelaylocal--;
    } else {
        timingdelaylocal = 1000U;
    }
}

/*!
    \brief    main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    int Run_mode  =  0;
    int Auto_flag = 0;
    

    AUTO_Threshod_Init();      //初始化阈值
    
    systick_config();
    timer_config(TIMER1, 1);	//1ms
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    gd_eval_com_init(USART0);  //printf
    
    BEEP_Init();
    BEEP_Install(200);    //BEEP持续200ms
    ZigBee_UART3_init(UART3);
    UART4_Lcd_Init(UART4);
    UART6_ESP8266_Init(UART6);
    USART5_IrDA_Init(USART5);
    /********************MultiTimer定时器**********************/
    MultiTimerInstall(systick_get);
    /* 启动定时器 */
	MultiTimerStart(&tim1, 10, timer1_callback, "ZigBee_Recvice");   //zigbee  10ms接收
    MultiTimerStart(&tim2, 2000, timer2_callback,"ZigBee_printf");    //zigbee  2s打印

    /********************MultiTimer定时器**********************/

    
    while(1) 
    {
        MultiTimerYield();//定时器
        
        Run_mode = UART_Lcd_Task(&Auto_Threshod);
        
        Change_mode(Run_mode,&Auto_flag);
        
        //ZigBee_Send_Recevice();
        Get_Val(Auto_flag,Auto_Threshod);
        
        IrDA_Send_Recevice(&Auto_Threshod,&IrDA_Buff);
        
        
        
    }
}
