#ifndef __ESP8266_H__
#define __ESP8266_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
#include <stdio.h>


#define COMn                             1U
#define ESP8266_COM0                        UART6
#define ESP8266_COM0_CLK                    RCU_UART6

#define ESP8266_COM0_TX_PIN                 GPIO_PIN_8   //PE8
#define ESP8266_COM0_RX_PIN                 GPIO_PIN_7    //PE7
        
#define ESP8266_COM0_GPIO_PORT              GPIOE
#define ESP8266_COM0_GPIO_CLK               RCU_GPIOE
#define ESP8266_COM0_AF                     GPIO_AF_8

typedef struct{
    char UART6_Buff[512];
    int UART6_Index;
    int UART6_Finish;
}ESP8266_Type;

extern ESP8266_Type ESP8266_Buff; 


void UART6_ESP8266_Init(uint32_t com);
void Send_ESP8266_String(uint8_t *str);
int ESP8266_SendCmd(char* cmd, char* reply, int wait);
int ESP8266_Init(void);
uint8_t ESP32ConnetWIFI(uint8_t *ssid,uint8_t *pwd);
uint8_t ESP32ConnetServer(uint8_t* tppe,uint8_t *remote_ip,uint8_t *remote_port);
void ExitPassThroughMode(void);
uint8_t ESP32Restore(void);
uint8_t ESP8266TcpInit(void);

#endif   //__ESP8266_H__


