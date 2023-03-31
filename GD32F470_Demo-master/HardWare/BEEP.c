#include "BEEP.h"
#include "systick.h"

void BEEP_Init(void)
{
    /* enable the led clock */
    rcu_periph_clock_enable(BEEP_GPIO_CLK);
    /* configure led GPIO port */ 
    gpio_mode_set(BEEP_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,BEEP_PIN);
    gpio_output_options_set(BEEP_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,BEEP_PIN);

    GPIO_BC(BEEP_GPIO_PORT) = BEEP_PIN;
    
}

void BEEP_Install(int time)
{
    BEEP_ON;
    delay_1ms(time);
    BEEP_OFF;
    delay_1ms(time);
}

