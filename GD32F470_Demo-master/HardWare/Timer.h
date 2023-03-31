#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
#include "main.h"


void timer_config(uint32_t timer_periph, uint32_t time_interval_ms);

#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H__ */
