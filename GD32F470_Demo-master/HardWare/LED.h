#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
     
/* exported types */
typedef enum 
{
    LED1 = 0,
    LED2 = 1,
} led_typedef_enum;

/* eval board low layer led */
#define LEDn                             2U

#define LED1_PIN                         GPIO_PIN_13
#define LED1_GPIO_PORT                   GPIOF
#define LED1_GPIO_CLK                    RCU_GPIOF
  
#define LED2_PIN                         GPIO_PIN_14
#define LED2_GPIO_PORT                   GPIOF
#define LED2_GPIO_CLK                    RCU_GPIOF
  
/* function declarations */
/* configures led GPIO */
void gd_eval_led_init(led_typedef_enum lednum);
/* turn on selected led */
void gd_eval_led_on(led_typedef_enum lednum);
/* turn off selected led */
void gd_eval_led_off(led_typedef_enum lednum);
/* toggle the selected led */
void gd_eval_led_toggle(led_typedef_enum lednum);



#ifdef __cplusplus
}
#endif

#endif /* __LED_H__ */
