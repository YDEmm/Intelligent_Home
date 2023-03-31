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
    usart_baudrate_set(IrDA_COM0,9600U);                    //���ò�����
    usart_parity_config(IrDA_COM0,USART_PM_NONE);             //����żУ��
    usart_word_length_set(IrDA_COM0,USART_WL_8BIT);           //8λ����λ
    usart_stop_bit_set(IrDA_COM0,USART_STB_1BIT);             //1λֹͣλ
    usart_receive_config(IrDA_COM0, USART_RECEIVE_ENABLE);    //ʹ�ܴ��ڽ���
    usart_transmit_config(IrDA_COM0, USART_TRANSMIT_ENABLE);  //ʹ�ܴ��ڷ���
    
    
    /* com init config */
    nvic_irq_enable(USART5_IRQn,0,0);
    
    usart_interrupt_flag_clear(USART5,USART_INT_FLAG_TC); /* ��������жϱ�־λ */
    usart_interrupt_enable(IrDA_COM0, USART_INT_RBNE);
    usart_interrupt_enable(IrDA_COM0, USART_INT_IDLE);
    usart_enable(IrDA_COM0);                                  //ʹ�ܴ���4
     
}
/* ���ڷ����ַ��� */
void Send_IrDA_String(uint8_t *str)
{
    //����һ���ֽ����ݵ�USART5
    while(*str != '\0')
    {
        usart_data_transmit(USART5,*str++);
        
        //�ȴ��������
        while(usart_flag_get(USART5,USART_FLAG_TBE) == RESET);
    }
}

IrDA_Type IrDA_Buff = {0};
int Confirm_Flag = 0;   //ȷ��״̬��־λ
void USART5_IRQHandler(void)
{
    if(usart_flag_get(IrDA_COM0, USART_FLAG_RBNE) != RESET)// &&(!usart6finish))   //�����ж�
    {
        usart_interrupt_flag_clear(IrDA_COM0, USART_INT_FLAG_RBNE);
        IrDA_Buff.data =  usart_data_receive(USART5);
    }
    else if(usart_flag_get(IrDA_COM0, USART_FLAG_IDLE) != RESET) //�����ж�
    {
        usart_interrupt_flag_clear(IrDA_COM0,USART_INT_FLAG_IDLE); /* ��������жϱ�־λ */
        usart_data_receive(USART5);
        IrDA_Buff.USART5_Finish = 1;
    }
}
























