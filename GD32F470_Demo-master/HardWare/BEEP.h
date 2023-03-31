#ifndef __BEEP_H__
#define __BEEP_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
     
#define BEEP_PIN                         GPIO_PIN_8
#define BEEP_GPIO_PORT                   GPIOC
#define BEEP_GPIO_CLK                    RCU_GPIOC

#define BEEP_ON                    GPIO_BOP(BEEP_GPIO_PORT) = BEEP_PIN;
#define BEEP_OFF                   GPIO_BC(BEEP_GPIO_PORT) = BEEP_PIN;

void BEEP_Init(void);
void BEEP_Install(int time);    
#ifdef __cplusplus
}
#endif

#endif /* __BEEP_H__ */
