#include "ESP8266.h"

static rcu_periph_enum COM_CLK[COMn] = {ESP8266_COM0_CLK};
static uint32_t COM_TX_PIN[COMn] = {ESP8266_COM0_TX_PIN};
static uint32_t COM_RX_PIN[COMn] = {ESP8266_COM0_RX_PIN};

void UART6_ESP8266_Init(uint32_t com)
{
    /* enable GPIO clock */
    uint32_t COM_ID = 0;
    if(ESP8266_COM0 == com)
    {
        COM_ID = 0U;
    }
    
    rcu_periph_clock_enable( ESP8266_COM0_GPIO_CLK);
    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[COM_ID]);
    /* connect port to USARTx_Tx */
    gpio_af_set(ESP8266_COM0_GPIO_PORT, ESP8266_COM0_AF, COM_TX_PIN[COM_ID]);
    /* connect port to USARTx_Rx */
    gpio_af_set(ESP8266_COM0_GPIO_PORT, ESP8266_COM0_AF, COM_RX_PIN[COM_ID]);
    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(ESP8266_COM0_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_TX_PIN[COM_ID]);
    gpio_output_options_set(ESP8266_COM0_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_TX_PIN[COM_ID]);
    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(ESP8266_COM0_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_RX_PIN[COM_ID]);
    gpio_output_options_set(ESP8266_COM0_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_RX_PIN[COM_ID]);

    /* USART configure */
    usart_deinit(ESP8266_COM0);
    usart_baudrate_set(ESP8266_COM0,115200U);                    //设置波特率
    usart_parity_config(ESP8266_COM0,USART_PM_NONE);             //无奇偶校验
    usart_word_length_set(ESP8266_COM0,USART_WL_8BIT);           //8位数据位
    usart_stop_bit_set(ESP8266_COM0,USART_STB_1BIT);             //1位停止位
    usart_receive_config(ESP8266_COM0, USART_RECEIVE_ENABLE);    //使能串口接收
    usart_transmit_config(ESP8266_COM0, USART_TRANSMIT_ENABLE);  //使能串口发送
    
    
    /* com init config */
    nvic_irq_enable(UART6_IRQn,0,0);
    
    usart_interrupt_flag_clear(UART6,USART_INT_FLAG_TC); /* 清除空闲中断标志位 */
    usart_interrupt_enable(ESP8266_COM0, USART_INT_RBNE);
    usart_interrupt_enable(ESP8266_COM0, USART_INT_IDLE);
    usart_enable(ESP8266_COM0);                                  //使能串口4
     
}
/* 串口发送字符串 */
void Send_ESP8266_String(uint8_t *str)
{
    //发送一个字节数据到USART0
    while(*str != '\0')
    {
        usart_data_transmit(UART6,*str++);
        
        //等待发送完毕
        while(usart_flag_get(UART6,USART_FLAG_TBE) == RESET);
    }
}

ESP8266_Type ESP8266_Buff; 
void UART6_IRQHandler(void)
{
    uint8_t data;
    if(usart_flag_get(ESP8266_COM0, USART_FLAG_RBNE) != RESET)// &&(!usart6finish))   //接收中断
    {
        usart_interrupt_flag_clear(ESP8266_COM0, USART_INT_FLAG_RBNE);
        if(ESP8266_Buff.UART6_Index < 510)
        {
            data =  usart_data_receive(UART6);
            ESP8266_Buff.UART6_Buff[ESP8266_Buff.UART6_Index++] = data;
        }
       
    }
    else if(usart_flag_get(ESP8266_COM0, USART_FLAG_IDLE) != RESET) //空闲中断
    {
        usart_interrupt_flag_clear(ESP8266_COM0,USART_INT_FLAG_IDLE); /* 清除空闲中断标志位 */
        usart_data_receive(UART6);
        ESP8266_Buff.UART6_Finish = 1;
    }
}




