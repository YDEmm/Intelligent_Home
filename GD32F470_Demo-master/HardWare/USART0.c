#include "USART0.h"
#include "string.h"
/* private variables */
static rcu_periph_enum COM_CLK[COMn] = {EVAL_COM0_CLK};
static uint32_t COM_TX_PIN[COMn] = {EVAL_COM0_TX_PIN};
static uint32_t COM_RX_PIN[COMn] = {EVAL_COM0_RX_PIN};

#define ARRAYNUM(arr_nanme)      (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))
#define TRANSMIT_SIZE            (ARRAYNUM(txbuffer) - 1)

char USART0_Tx_buffer[256] = {0};
char USART0_Rx_buffer[256] = {0};
uint8_t USART0_RX_Counter = 0;

/*!
    \brief    configure COM port
    \param[in]  COM: COM on the board
      \arg        EVAL_COM0: COM on the board
    \param[out] none
    \retval     none
*/
void gd_eval_com_init(uint32_t com)
{
    /* enable GPIO clock */
    uint32_t COM_ID = 0;
    if(EVAL_COM0 == com)
    {
        COM_ID = 0U;
    }

    rcu_periph_clock_enable( EVAL_COM0_GPIO_CLK);
    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[COM_ID]);
    /* connect port to USARTx_Tx */
    gpio_af_set(EVAL_COM0_GPIO_PORT, EVAL_COM0_AF, COM_TX_PIN[COM_ID]);
    /* connect port to USARTx_Rx */
    gpio_af_set(EVAL_COM0_GPIO_PORT, EVAL_COM0_AF, COM_RX_PIN[COM_ID]);
    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(EVAL_COM0_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_TX_PIN[COM_ID]);
    gpio_output_options_set(EVAL_COM0_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_TX_PIN[COM_ID]);
    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(EVAL_COM0_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_RX_PIN[COM_ID]);
    gpio_output_options_set(EVAL_COM0_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_RX_PIN[COM_ID]);

    /* USART configure */
    usart_deinit(EVAL_COM0);
    usart_baudrate_set(EVAL_COM0,115200U);                    //���ò�����
    usart_parity_config(EVAL_COM0,USART_PM_NONE);             //����żУ��
    usart_word_length_set(EVAL_COM0,USART_WL_8BIT);           //8λ����λ
    usart_stop_bit_set(EVAL_COM0,USART_STB_1BIT);             //1λֹͣλ
    usart_receive_config(EVAL_COM0, USART_RECEIVE_ENABLE);    //ʹ�ܴ��ڽ���
    usart_transmit_config(EVAL_COM0, USART_TRANSMIT_ENABLE);  //ʹ�ܴ��ڷ���
    usart_enable(EVAL_COM0);                                  //ʹ�ܴ���
    
    /* com init config */
    
    usart_interrupt_enable(EVAL_COM0, USART_INT_RBNE);
    usart_interrupt_enable(EVAL_COM0,USART_INT_TBE);
    //nvic_irq_enable(USART0_IRQn,0,0);
}

/* ���ڷ���һ���ֽ� */
void USART0_Send_Byte(uint8_t ch)
{
    //����һ���ֽ����ݵ�USART0
    usart_data_transmit(USART0,ch);
    
    //�ȴ��������
    while(usart_flag_get(USART0,USART_FLAG_TBE) == RESET);
    
}
/* ���ڷ����ַ�����ֱ�������ַ��������� */
void USART0_Send_String(uint8_t *str)
{
    unsigned int k = 0;
    do
    {
        USART0_Send_Byte(*(str + k));
        k++;
    }while(*(str + k) != '\0');
}

void USART0_IRQHandler(void)
{

    if (RESET != usart_interrupt_flag_get(EVAL_COM0, USART_INT_FLAG_RBNE))
    {
        usart_interrupt_flag_clear(EVAL_COM0,USART_INT_FLAG_RBNE); /* ��������жϱ�־λ */
        usart_data_receive(EVAL_COM0);                             /* ���������ɱ�־λ */
        usart_flag_get(EVAL_COM0, USART_FLAG_TC);                   /* �����־λ */
        USART0_Rx_buffer[USART0_RX_Counter++] = (uint8_t)usart_data_receive(EVAL_COM0);
        
    }
    
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    //�����־λ
    usart_flag_clear(EVAL_COM0,USART_FLAG_TC);
    //����һ���ֽ����ݵ�USART0
    usart_data_transmit(EVAL_COM0,(uint8_t)ch);
    //�ȴ��������
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE));
    return ch;
}
/* retarget the C library scanf function to the USART */
int fgetc(FILE *f)
{
    //�ȴ�USART0��������
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_RBNE));
    return (int)usart_data_receive(EVAL_COM0);
}

