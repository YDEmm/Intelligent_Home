#include "ZigBee_UART3.h"
#include "systick.h"
#include "USART0.h"
/* private variables */
static rcu_periph_enum COM_CLK[COMn] = {ZigBee_COM4_CLK};
static uint32_t COM_TX_PIN[COMn] = {ZigBee_COM4_TX_PIN};
static uint32_t COM_RX_PIN[COMn] = {ZigBee_COM4_RX_PIN};

#define UART3_DATA_ADDRESS      ((uint32_t)&USART_DATA(UART3))
#define ARRAYNUM(arr_nanme)      (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))

__IO FlagStatus g_transfer_complete = RESET;
char UART3_Tx_DMA_buffer[256] = {0};
char UART3_Rx_DMA_buffer[256] = {0};
char Subg_UART3_Rx_Data[256] = {0};

uint32_t Rx_DMA_BuffNum = 0;
uint8_t ZigBee_Send_Flag = 0;
uint8_t ZigBee_Recvice_Flag = 0;

void nvic_config(void);
void dma_config(uint32_t com);

/*!
    \brief    configure COM port
    \param[in]  COM: COM on the board
      \arg        EVAL_COM0: COM on the board
*/
void ZigBee_UART3_init(uint32_t com)
{
    ZigBee_Send_Flag = 1;
    dma_single_data_parameter_struct dma_init_struct;   /* DMA���ò����ṹ�� */
    /* enable GPIO clock */
    uint32_t COM_ID = 0;
    if(ZigBee_COM4 == com)
    {
        COM_ID = 0U;
    }
    rcu_periph_clock_enable( ZigBee_COM4_GPIO_CLK);

    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[COM_ID]);
    rcu_periph_clock_enable(RCU_DMA0);      /* ʹ�� DMA0 ʱ�� */

    /* connect port to USARTx_Tx */
    gpio_af_set(ZigBee_COM4_GPIO_PORT, ZigBee_COM4_AF, COM_TX_PIN[COM_ID]);

    /* connect port to USARTx_Rx */
    gpio_af_set(ZigBee_COM4_GPIO_PORT, ZigBee_COM4_AF, COM_RX_PIN[COM_ID]);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(ZigBee_COM4_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_TX_PIN[COM_ID]);
    gpio_output_options_set(ZigBee_COM4_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_TX_PIN[COM_ID]);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(ZigBee_COM4_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_RX_PIN[COM_ID]);
    gpio_output_options_set(ZigBee_COM4_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_RX_PIN[COM_ID]);

    /* USART configure */
    usart_deinit(com);
    usart_baudrate_set(com,115200U);                    //���ò�����
    usart_parity_config(com,USART_PM_NONE);             //����żУ��
    usart_word_length_set(com,USART_WL_8BIT);           //8λ����λ
    usart_stop_bit_set(com,USART_STB_1BIT);             //1λֹͣλ
    usart_receive_config(com, USART_RECEIVE_ENABLE);    //ʹ�ܴ��ڽ���
    usart_transmit_config(com, USART_TRANSMIT_ENABLE);  //ʹ�ܴ��ڷ���
    usart_enable(com);                                  //ʹ�ܴ���
    
    /* deinitialize DMA0 channel4(UART3 TX) */
    dma_single_data_para_struct_init(&dma_init_struct);
    dma_deinit(DMA0, DMA_CH4);
    
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;//���䷽�� ���ڴ������跢������
    
    dma_init_struct.memory0_addr = (uint32_t)UART3_Tx_DMA_buffer;//�洢������ַ���ã�����������ַ��buff��ַ
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;//�洢����ַ������ʽ��Ϊ�̶�
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.number = ARRAYNUM(UART3_Tx_DMA_buffer);//���ͨ�����ݼĴ����Ĵ�С�Ļ������ĳ���
    dma_init_struct.periph_addr = UART3_DATA_ADDRESS;//�������ַ����ĳĳĳ���ܵ����ݼĴ�������ַ������ADC0.����USART1�����ݼĴ�������ַ
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;//�����ַ������ʽ��Ϊ�̶�
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH4, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH4);                     //�ر�DMAѭ��ģʽ
    dma_channel_subperipheral_select(DMA0, DMA_CH4, DMA_SUBPERI4);
    /* enable DMA0 channel4 transfer complete interrupt */
    dma_interrupt_enable(DMA0, DMA_CH4, DMA_CHXCTL_FTFIE);
    /* enable DMA0 channel4 */
    dma_channel_enable(DMA0, DMA_CH4);

    /* deinitialize DMA0 channel2 (UART3 RX) */
    dma_deinit(DMA0, DMA_CH2);
    dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;           //���赽�ڴ�
    dma_init_struct.memory0_addr = (uint32_t)UART3_Rx_DMA_buffer;     //�����ڴ���ջ���ַ
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;    //�ڴ��ַ����
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;//8λ��ַ����
    dma_init_struct.number = sizeof(UART3_Rx_DMA_buffer);   
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(UART3); //�������ַ,USART���ݼĴ�����ַ
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;   //�����ַ������
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;         //���DMAͨ�����ȼ�
    dma_single_data_mode_init(DMA0, DMA_CH2, &dma_init_struct); //���սṹ������ó�ʼ��DMA

    /* configure DMA mode */
    dma_circulation_enable(DMA0, DMA_CH2);                     //����DMAѭ��ģʽ -----������ѭ��ģʽ
    dma_channel_subperipheral_select(DMA0, DMA_CH2, DMA_SUBPERI4);//
    /* enable DMA0 channel2 transfer complete interrupt */
    dma_interrupt_enable(DMA0, DMA_CH2, DMA_CHXCTL_FTFIE);
    /* enable DMA0 channel2 */
    dma_channel_enable(DMA0, DMA_CH2);
    
    /* USART DMA enable*/
    usart_dma_receive_config(UART3, USART_RECEIVE_DMA_ENABLE);  /* UART3 DMA����ģʽ���� */
    //usart_dma_transmit_config(UART3, USART_TRANSMIT_DMA_ENABLE);

    nvic_irq_enable(UART3_IRQn, 0, 0);     /* USART�ж����ã���ռ���ȼ�0�������ȼ�0 */
    usart_interrupt_enable(UART3, USART_INT_IDLE);         /* ʹ��UART3�����ж� */
}

void UART3_DMA_Send(char *buffer)
{ 
    buffer = UART3_Tx_DMA_buffer;   //��ζ�����еĴ���3����ֻ�ܷ����ַ���UART3_Tx_DMA_buffer�������
    dma_channel_disable(DMA0, DMA_CH4);
    dma_flag_clear(DMA0,DMA_CH4,DMA_INT_FLAG_FTF);   
    dma_memory_address_config(DMA0, DMA_CH4,DMA_MEMORY_0,(uint32_t)buffer);//����Ҫ�������ݵ��ڴ��ַ
    dma_transfer_number_config(DMA0, DMA_CH4,sizeof(UART3_Tx_DMA_buffer));//һ�������ٸ�����
    
    dma_channel_enable(DMA0, DMA_CH4);
    delay_1ms(3);
    /* USART DMA enable for transmission and reception */
    usart_dma_transmit_config(UART3, USART_TRANSMIT_DMA_ENABLE);//ʹ�ܴ���DMA����
    while(RESET == dma_flag_get(DMA0, DMA_CH4, DMA_FLAG_FTF));
}

void UART3_IRQHandler(void)
{
    //DMA���գ�ֻ���������ж�-----�������ڿ����жϵ�ʱ�򣬱��������Ѿ��������
    //��Ҫ�������ݴ���ʱ������⣬Ҳ���ǵ����շ���ʼ����DMA�󣬷��ͷ�����������û��
    if(usart_flag_get(ZigBee_COM4, USART_FLAG_IDLE) != RESET)   //�����ж�
    {
        dma_channel_disable(DMA0, DMA_CH2);                    /* �ر�DMA���� */
        
        usart_interrupt_flag_clear(UART3,USART_INT_FLAG_IDLE); /* ��������жϱ�־λ */
        usart_data_receive(UART3);                             /* ���������ɱ�־λ */
        usart_flag_get(ZigBee_COM4, USART_FLAG_TC);            /* �����־λ */
        dma_interrupt_flag_clear(DMA0,DMA_CH2,DMA_INT_FLAG_FTF);
         
        Rx_DMA_BuffNum = sizeof(UART3_Rx_DMA_buffer) - dma_transfer_number_get(DMA0,DMA_CH2);
        memcpy(Subg_UART3_Rx_Data,UART3_Rx_DMA_buffer,Rx_DMA_BuffNum); /* ת�����ݵ����������ݻ�����*/
        Subg_UART3_Rx_Data[Rx_DMA_BuffNum] = '\0'; /* ����ַ��������� */

        
        ZigBee_Recvice_Flag = 1;  //���ձ�־λ

        /* ��������DMA���� */
        dma_memory_address_config(DMA0,DMA_CH2,DMA_MEMORY_0,(uint32_t)UART3_Rx_DMA_buffer);
        dma_transfer_number_config(DMA0,DMA_CH2,sizeof(UART3_Rx_DMA_buffer));
        dma_channel_enable(DMA0, DMA_CH2);  /* ����DMA���� */
    }
    
}


