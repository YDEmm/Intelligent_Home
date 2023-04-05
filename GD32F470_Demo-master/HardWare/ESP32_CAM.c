#include "ESP32_CAM.h"
#include "string.h"
/* private variables */
static rcu_periph_enum COM_CLK[COMn] = {ESP32_CAM_COM0_CLK};
static uint32_t COM_TX_PIN[COMn] = {ESP32_CAM_COM0_TX_PIN};
static uint32_t COM_RX_PIN[COMn] = {ESP32_CAM_COM0_RX_PIN};

#define ARRAYNUM(arr_nanme)      (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))
#define TRANSMIT_SIZE            (ARRAYNUM(txbuffer) - 1)

char USART2_Tx_buffer[256] = {0};
char USART2_Rx_buffer[256] = {0};
uint8_t USART2_RX_Counter = 0;

/*!
    \brief    configure COM port
    \param[in]  COM: COM on the board
      \arg        EVAL_COM0: COM on the board
    \param[out] none
    \retval     none
*/
void ESP32_CAM_USART2_Init(uint32_t com)
{
    /* enable GPIO clock */
    uint32_t COM_ID = 0;
    if(ESP32_CAM_COM0 == com)
    {
        COM_ID = 0U;
    }

    rcu_periph_clock_enable( ESP32_CAM_COM0_GPIO_CLK);
    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[COM_ID]);
    /* connect port to USARTx_Tx */
    gpio_af_set(ESP32_CAM_COM0_GPIO_PORT, ESP32_CAM_COM0_AF, COM_TX_PIN[COM_ID]);
    /* connect port to USARTx_Rx */
    gpio_af_set(ESP32_CAM_COM0_GPIO_PORT, ESP32_CAM_COM0_AF, COM_RX_PIN[COM_ID]);
    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(ESP32_CAM_COM0_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_TX_PIN[COM_ID]);
    gpio_output_options_set(ESP32_CAM_COM0_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_TX_PIN[COM_ID]);
    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(ESP32_CAM_COM0_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_RX_PIN[COM_ID]);
    gpio_output_options_set(ESP32_CAM_COM0_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_RX_PIN[COM_ID]);

    /* USART configure */
    usart_deinit(ESP32_CAM_COM0);
    usart_baudrate_set(ESP32_CAM_COM0,115200U);                    //设置波特率
    usart_parity_config(ESP32_CAM_COM0,USART_PM_NONE);             //无奇偶校验
    usart_word_length_set(ESP32_CAM_COM0,USART_WL_8BIT);           //8位数据位
    usart_stop_bit_set(ESP32_CAM_COM0,USART_STB_1BIT);             //1位停止位
    usart_receive_config(ESP32_CAM_COM0, USART_RECEIVE_ENABLE);    //使能串口接收
    usart_transmit_config(ESP32_CAM_COM0, USART_TRANSMIT_ENABLE);  //使能串口发送
    usart_enable(ESP32_CAM_COM0);                                  //使能串口
    
    /* com init config */
    
    usart_interrupt_enable(ESP32_CAM_COM0, USART_INT_RBNE);
    usart_interrupt_enable(ESP32_CAM_COM0,USART_INT_TBE);
    nvic_irq_enable(USART2_IRQn,0,0);
}

/* 串口发送一个字节 */
void USART2_Send_Byte(uint8_t ch)
{
    //发送一个字节数据到USART0
    usart_data_transmit(USART2,ch);
    
    //等待发送完毕
    while(usart_flag_get(USART2,USART_FLAG_TBE) == RESET);
    
}
/* 串口发送字符串，直到遇到字符穿结束符 */
void USART2_Send_String(uint8_t *str)
{
    unsigned int k = 0;
    do
    {
        USART2_Send_Byte(*(str + k));
        k++;
    }while(*(str + k) != '\0');
}

void USART2_IRQHandler(void)
{

    if (RESET != usart_interrupt_flag_get(ESP32_CAM_COM0, USART_INT_FLAG_RBNE))
    {
        usart_interrupt_flag_clear(ESP32_CAM_COM0,USART_INT_FLAG_RBNE); /* 清除空闲中断标志位 */
        usart_data_receive(ESP32_CAM_COM0);                             /* 清除接收完成标志位 */
        usart_flag_get(ESP32_CAM_COM0, USART_FLAG_TC);                  /* 清除标志位 */
        USART2_Rx_buffer[USART2_RX_Counter++] = (uint8_t)usart_data_receive(ESP32_CAM_COM0);
        
    }
    
}



