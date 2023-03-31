#include "Timer.h"

void timer_config(uint32_t timer_periph, uint32_t time_interval_ms) 
{
    timer_parameter_struct timer_initpara;

    switch(timer_periph){
    case TIMER0:
    	rcu_periph_clock_enable(RCU_TIMER0);
    	break;
    case TIMER1:
        rcu_periph_clock_enable(RCU_TIMER1);
        break;
    case TIMER2:
        rcu_periph_clock_enable(RCU_TIMER2);
        break;
    case TIMER3:
        rcu_periph_clock_enable(RCU_TIMER3);
        break;
    case TIMER4:
        rcu_periph_clock_enable(RCU_TIMER4);
        break;
    case TIMER5:
        rcu_periph_clock_enable(RCU_TIMER5);
        break;
    case TIMER6:
        rcu_periph_clock_enable(RCU_TIMER6);
        break;
    default:
    	break;
    }

    timer_deinit(timer_periph);
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler         = 19999;	//200M/20000 = 10K Hz
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = (uint32_t)10*time_interval_ms;//(uint32_t)1000000U/time_interval_us;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(timer_periph, &timer_initpara);

    timer_interrupt_enable(timer_periph, TIMER_INT_UP);	//update interrupt
    timer_enable(timer_periph);
    nvic_irq_enable(TIMER1_IRQn,1,0);
}

extern void systick_run(void);
void TIMER1_IRQHandler(void) 
{
	if(SET == timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP)){
		/* clear channel 0 interrupt bit */
		timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
		/*********MultiTimer¶¨Ê±Æ÷**********/
        systick_run();
	}
}











