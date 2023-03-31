#include "IrDA.h"
#include "function.h"


static rcu_periph_enum COM_CLK[COMn] = {IrDA_COM0_CLK};
static uint32_t COM_TX_PIN[COMn] = {IrDA_COM0_TX_PIN};
static uint32_t COM_RX_PIN[COMn] = {IrDA_COM0_RX_PIN};

void USART5_IrDA_Init(uint32_t com)
{
    /* enable GPIO clock */
    uint32_t COM_ID = 0;
    if(IrDA_COM0 == com)
    {
        COM_ID = 0U;
    }
    
    rcu_periph_clock_enable( IrDA_COM0_GPIO_CLK);
    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[COM_ID]);
    /* connect port to USARTx_Tx */
    gpio_af_set(IrDA_COM0_GPIO_PORT, IrDA_COM0_AF, COM_TX_PIN[COM_ID]);
    /* connect port to USARTx_Rx */
    gpio_af_set(IrDA_COM0_GPIO_PORT, IrDA_COM0_AF, COM_RX_PIN[COM_ID]);
    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(IrDA_COM0_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_TX_PIN[COM_ID]);
    gpio_output_options_set(IrDA_COM0_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_TX_PIN[COM_ID]);
    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(IrDA_COM0_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_RX_PIN[COM_ID]);
    gpio_output_options_set(IrDA_COM0_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_RX_PIN[COM_ID]);

    /* USART configure */
    usart_deinit(IrDA_COM0);
    usart_baudrate_set(IrDA_COM0,9600U);                    //设置波特率
    usart_parity_config(IrDA_COM0,USART_PM_NONE);             //无奇偶校验
    usart_word_length_set(IrDA_COM0,USART_WL_8BIT);           //8位数据位
    usart_stop_bit_set(IrDA_COM0,USART_STB_1BIT);             //1位停止位
    usart_receive_config(IrDA_COM0, USART_RECEIVE_ENABLE);    //使能串口接收
    usart_transmit_config(IrDA_COM0, USART_TRANSMIT_ENABLE);  //使能串口发送
    
    
    /* com init config */
    nvic_irq_enable(USART5_IRQn,0,0);
    
    usart_interrupt_flag_clear(USART5,USART_INT_FLAG_TC); /* 清除空闲中断标志位 */
    usart_interrupt_enable(IrDA_COM0, USART_INT_RBNE);
    usart_interrupt_enable(IrDA_COM0, USART_INT_IDLE);
    usart_enable(IrDA_COM0);                                  //使能串口4
     
}
/* 串口发送字符串 */
void Send_IrDA_String(uint8_t *str)
{
    //发送一个字节数据到USART5
    while(*str != '\0')
    {
        usart_data_transmit(USART5,*str++);
        
        //等待发送完毕
        while(usart_flag_get(USART5,USART_FLAG_TBE) == RESET);
    }
}

IrDA_Type IrDA_Buff = {0};
int Confirm_Flag = 0;   //确认状态标志位
void USART5_IRQHandler(void)
{
    if(usart_flag_get(IrDA_COM0, USART_FLAG_RBNE) != RESET)// &&(!usart6finish))   //接收中断
    {
        usart_interrupt_flag_clear(IrDA_COM0, USART_INT_FLAG_RBNE);
        IrDA_Buff.data =  usart_data_receive(USART5);
    }
    else if(usart_flag_get(IrDA_COM0, USART_FLAG_IDLE) != RESET) //空闲中断
    {
        usart_interrupt_flag_clear(IrDA_COM0,USART_INT_FLAG_IDLE); /* 清除空闲中断标志位 */
        usart_data_receive(USART5);
        IrDA_Buff.USART5_Finish = 1;
    }
}
























