#ifndef __KEY_H__
#define __KEY_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
     
/* exported types */
typedef enum 
{
    KEY_WAKEUP = 0,
    KEY_TAMPER = 1,
    KEY_USER = 2
} key_typedef_enum;

typedef enum 
{
    KEY_MODE_GPIO = 0,
    KEY_MODE_EXTI = 1
} keymode_typedef_enum;

#define KEYn                             4U

/* tamper push-button */
#define TAMPER_KEY_PIN                   GPIO_PIN_13
#define TAMPER_KEY_GPIO_PORT             GPIOC
#define TAMPER_KEY_GPIO_CLK              RCU_GPIOC
#define TAMPER_KEY_EXTI_LINE             EXTI_13
#define TAMPER_KEY_EXTI_PORT_SOURCE      EXTI_SOURCE_GPIOC
#define TAMPER_KEY_EXTI_PIN_SOURCE       EXTI_SOURCE_PIN13
#define TAMPER_KEY_EXTI_IRQn             EXTI10_15_IRQn

/* wakeup push-button */
#define WAKEUP_KEY_PIN                   GPIO_PIN_0
#define WAKEUP_KEY_GPIO_PORT             GPIOA
#define WAKEUP_KEY_GPIO_CLK              RCU_GPIOA
#define WAKEUP_KEY_EXTI_LINE             EXTI_0
#define WAKEUP_KEY_EXTI_PORT_SOURCE      EXTI_SOURCE_GPIOA
#define WAKEUP_KEY_EXTI_PIN_SOURCE       EXTI_SOURCE_PIN0
#define WAKEUP_KEY_EXTI_IRQn             EXTI0_IRQn  

/* user push-button */
#define USER_KEY0_PIN                     GPIO_PIN_0
#define USER_KEY0_GPIO_PORT               GPIOD
#define USER_KEY0_GPIO_CLK                RCU_GPIOD
#define USER_KEY0_EXTI_LINE               EXTI_0
#define USER_KEY0_EXTI_PORT_SOURCE        EXTI_SOURCE_GPIOD
#define USER_KEY0_EXTI_PIN_SOURCE         EXTI_SOURCE_PIN0
#define USER_KEY0_EXTI_IRQn               EXTI0_IRQn

#define USER_KEY1_PIN                     GPIO_PIN_1
#define USER_KEY1_GPIO_PORT               GPIOD
#define USER_KEY1_GPIO_CLK                RCU_GPIOD
#define USER_KEY1_EXTI_LINE               EXTI_1
#define USER_KEY1_EXTI_PORT_SOURCE        EXTI_SOURCE_GPIOD
#define USER_KEY1_EXTI_PIN_SOURCE         EXTI_SOURCE_PIN1
#define USER_KEY1_EXTI_IRQn               EXTI1_IRQn
  
/* function declarations */
/* configure key */
void gd_eval_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode);
/* return the selected button state */
uint8_t gd_eval_key_state_get(key_typedef_enum button);



#ifdef __cplusplus
}
#endif

#endif /* __KEY_H__ */
