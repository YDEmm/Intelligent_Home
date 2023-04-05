#include "KEY.h"
#include "systick.h"
#include <string.h>
#include "LED.h"
#include "ESP8266.h"
/* private variables */

static uint32_t KEY_PORT[KEYn] = {WAKEUP_KEY_GPIO_PORT, 
                                  TAMPER_KEY_GPIO_PORT,
                                  USER_KEY0_GPIO_PORT,USER_KEY1_GPIO_PORT};
static uint32_t KEY_PIN[KEYn] = {WAKEUP_KEY_PIN, TAMPER_KEY_PIN,USER_KEY0_PIN,USER_KEY1_PIN};
static rcu_periph_enum KEY_CLK[KEYn] = {WAKEUP_KEY_GPIO_CLK, 
                                        TAMPER_KEY_GPIO_CLK,
                                        USER_KEY0_GPIO_CLK,USER_KEY1_GPIO_CLK};
static exti_line_enum KEY_EXTI_LINE[KEYn] = {WAKEUP_KEY_EXTI_LINE,
                                             TAMPER_KEY_EXTI_LINE,
                                             USER_KEY0_EXTI_LINE,USER_KEY1_EXTI_LINE};
static uint8_t KEY_PORT_SOURCE[KEYn] = {WAKEUP_KEY_EXTI_PORT_SOURCE,
                                        TAMPER_KEY_EXTI_PORT_SOURCE,
                                        USER_KEY0_EXTI_PORT_SOURCE,USER_KEY1_EXTI_PORT_SOURCE};
static uint8_t KEY_PIN_SOURCE[KEYn] = {WAKEUP_KEY_EXTI_PIN_SOURCE,
                                       TAMPER_KEY_EXTI_PIN_SOURCE,
                                       USER_KEY0_EXTI_PIN_SOURCE,USER_KEY1_EXTI_PIN_SOURCE};
static uint8_t KEY_IRQn[KEYn] = {WAKEUP_KEY_EXTI_IRQn, 
                                 TAMPER_KEY_EXTI_IRQn,
                                 USER_KEY0_EXTI_IRQn,USER_KEY1_EXTI_IRQn};
/*!
    \brief    configure key
    \param[in]  key_num: specify the key to be configured
      \arg        KEY_TAMPER: tamper key
      \arg        KEY_WAKEUP: wakeup key
      \arg        KEY_USER: user key
    \param[in]  key_mode: specify button mode
      \arg        KEY_MODE_GPIO: key will be used as simple IO
      \arg        KEY_MODE_EXTI: key will be connected to EXTI line with interrupt
    \param[out] none
    \retval     none
*/
void gd_eval_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode)
{

    /* enable the key clock */

    rcu_periph_clock_enable(KEY_CLK[key_num]);   //开IO时钟
    rcu_periph_clock_enable(RCU_SYSCFG);         //外部中断时钟

    /* configure button pin as input */
    gpio_mode_set(KEY_PORT[key_num], GPIO_MODE_INPUT, GPIO_PUPD_PULLUP,KEY_PIN[key_num]);  //上拉输入

    if (key_mode == KEY_MODE_EXTI) {
        /* enable and set key EXTI interrupt to the lowest priority */
        nvic_irq_enable(KEY_IRQn[key_num], 2U, 0U);

        /* connect key EXTI line to key GPIO pin */
        syscfg_exti_line_config(KEY_PORT_SOURCE[key_num], KEY_PIN_SOURCE[key_num]);

        /* configure key EXTI line */
        exti_init(KEY_EXTI_LINE[key_num], EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(KEY_EXTI_LINE[key_num]);
    }
}

/*!
    \brief    return the selected button state
    \param[in]  button: specify the button to be checked
      \arg        KEY_TAMPER: tamper key
      \arg        KEY_WAKEUP: wakeup key
      \arg        KEY_USER: user key
    \param[out] none
    \retval     the button GPIO pin value
*/
uint8_t gd_eval_key_state_get(key_typedef_enum button)
{
    return gpio_input_bit_get(KEY_PORT[button], KEY_PIN[button]);
}

int KEY1_Flag = 0;
void  EXTI0_IRQHandler(void)
{
    if(RESET != exti_interrupt_flag_get(EXTI_0)){
       /*--------以下放入中断响应函数---------*/ 
        KEY1_Flag = 1;
 
    }
    exti_interrupt_flag_clear(EXTI_0);
}
int KEY2_Flag = 0;
void  EXTI1_IRQHandler(void)
{
    if(RESET != exti_interrupt_flag_get(EXTI_1)){
       /*--------以下放入中断响应函数---------*/ 
        KEY2_Flag = 1;
 
    }
    exti_interrupt_flag_clear(EXTI_1);
    
}
void Key_Control(void)
{
	if(KEY1_Flag == 1)  
    {
        gd_eval_led_on(LED2);
        delay_1ms(1000);
        KEY1_Flag = 0;  //清除标志位
    }
    else
    {
        gd_eval_led_off(LED2);
    }
    if(KEY2_Flag == 1)
    {
        gd_eval_led_on(LED1);
        ESP8266_SendCmd("AT+CIPCLOSE\r\n","OK\r\n",1000);//断开连接
        delay_1ms(1000);
        KEY2_Flag = 0;  //清除标志位
    }
    else
    {
        gd_eval_led_off(LED1);
    }
}


