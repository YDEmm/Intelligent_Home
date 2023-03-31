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
    dma_single_data_parameter_struct dma_init_struct;   /* DMA配置参数结构体 */
    /* enable GPIO clock */
    uint32_t COM_ID = 0;
    if(ZigBee_COM4 == com)
    {
        COM_ID = 0U;
    }
    rcu_periph_clock_enable( ZigBee_COM4_GPIO_CLK);

    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[COM_ID]);
    rcu_periph_clock_enable(RCU_DMA0);      /* 使能 DMA0 时钟 */

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
    usart_baudrate_set(com,115200U);                    //设置波特率
    usart_parity_config(com,USART_PM_NONE);             //无奇偶校验
    usart_word_length_set(com,USART_WL_8BIT);           //8位数据位
    usart_stop_bit_set(com,USART_STB_1BIT);             //1位停止位
    usart_receive_config(com, USART_RECEIVE_ENABLE);    //使能串口接收
    usart_transmit_config(com, USART_TRANSMIT_ENABLE);  //使能串口发送
    usart_enable(com);                                  //使能串口
    
    /* deinitialize DMA0 channel4(UART3 TX) */
    dma_single_data_para_struct_init(&dma_init_struct);
    dma_deinit(DMA0, DMA_CH4);
    
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;//传输方向 主内存向外设发送数据
    
    dma_init_struct.memory0_addr = (uint32_t)UART3_Tx_DMA_buffer;//存储器基地址设置，即缓冲区地址，buff地址
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;//存储器地址增长方式，为固定
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.number = ARRAYNUM(UART3_Tx_DMA_buffer);//相对通道数据寄存器的大小的缓冲区的长度
    dma_init_struct.periph_addr = UART3_DATA_ADDRESS;//外设基地址，即某某某功能的数据寄存器基地址，比如ADC0.比如USART1的数据寄存器基地址
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;//外设地址增长方式，为固定
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH4, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH4);                     //关闭DMA循环模式
    dma_channel_subperipheral_select(DMA0, DMA_CH4, DMA_SUBPERI4);
    /* enable DMA0 channel4 transfer complete interrupt */
    dma_interrupt_enable(DMA0, DMA_CH4, DMA_CHXCTL_FTFIE);
    /* enable DMA0 channel4 */
    dma_channel_enable(DMA0, DMA_CH4);

    /* deinitialize DMA0 channel2 (UART3 RX) */
    dma_deinit(DMA0, DMA_CH2);
    dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;           //外设到内存
    dma_init_struct.memory0_addr = (uint32_t)UART3_Rx_DMA_buffer;     //设置内存接收基地址
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;    //内存地址递增
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;//8位地址数据
    dma_init_struct.number = sizeof(UART3_Rx_DMA_buffer);   
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(UART3); //外设基地址,USART数据寄存器地址
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;   //外设地址不递增
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;         //最高DMA通道优先级
    dma_single_data_mode_init(DMA0, DMA_CH2, &dma_init_struct); //按照结构体的配置初始化DMA

    /* configure DMA mode */
    dma_circulation_enable(DMA0, DMA_CH2);                     //开启DMA循环模式 -----必须是循环模式
    dma_channel_subperipheral_select(DMA0, DMA_CH2, DMA_SUBPERI4);//
    /* enable DMA0 channel2 transfer complete interrupt */
    dma_interrupt_enable(DMA0, DMA_CH2, DMA_CHXCTL_FTFIE);
    /* enable DMA0 channel2 */
    dma_channel_enable(DMA0, DMA_CH2);
    
    /* USART DMA enable*/
    usart_dma_receive_config(UART3, USART_RECEIVE_DMA_ENABLE);  /* UART3 DMA接收模式开启 */
    //usart_dma_transmit_config(UART3, USART_TRANSMIT_DMA_ENABLE);

    nvic_irq_enable(UART3_IRQn, 0, 0);     /* USART中断设置，抢占优先级0，子优先级0 */
    usart_interrupt_enable(UART3, USART_INT_IDLE);         /* 使能UART3空闲中断 */
}

void UART3_DMA_Send(char *buffer)
{ 
    buffer = UART3_Tx_DMA_buffer;   //意味着所有的串口3发送只能发送字符串UART3_Tx_DMA_buffer里的内容
    dma_channel_disable(DMA0, DMA_CH4);
    dma_flag_clear(DMA0,DMA_CH4,DMA_INT_FLAG_FTF);   
    dma_memory_address_config(DMA0, DMA_CH4,DMA_MEMORY_0,(uint32_t)buffer);//设置要发送数据的内存地址
    dma_transfer_number_config(DMA0, DMA_CH4,sizeof(UART3_Tx_DMA_buffer));//一共发多少个数据
    
    dma_channel_enable(DMA0, DMA_CH4);
    delay_1ms(3);
    /* USART DMA enable for transmission and reception */
    usart_dma_transmit_config(UART3, USART_TRANSMIT_DMA_ENABLE);//使能串口DMA发送
    while(RESET == dma_flag_get(DMA0, DMA_CH4, DMA_FLAG_FTF));
}

void UART3_IRQHandler(void)
{
    //DMA接收，只触发空闲中断-----发生串口空闲中断的时候，表明数据已经接收完成
    //需要考虑数据传输时间差问题，也就是当接收方初始化完DMA后，发送方发送了数据没有
    if(usart_flag_get(ZigBee_COM4, USART_FLAG_IDLE) != RESET)   //空闲中断
    {
        dma_channel_disable(DMA0, DMA_CH2);                    /* 关闭DMA传输 */
        
        usart_interrupt_flag_clear(UART3,USART_INT_FLAG_IDLE); /* 清除空闲中断标志位 */
        usart_data_receive(UART3);                             /* 清除接收完成标志位 */
        usart_flag_get(ZigBee_COM4, USART_FLAG_TC);            /* 清除标志位 */
        dma_interrupt_flag_clear(DMA0,DMA_CH2,DMA_INT_FLAG_FTF);
         
        Rx_DMA_BuffNum = sizeof(UART3_Rx_DMA_buffer) - dma_transfer_number_get(DMA0,DMA_CH2);
        memcpy(Subg_UART3_Rx_Data,UART3_Rx_DMA_buffer,Rx_DMA_BuffNum); /* 转存数据到待处理数据缓冲区*/
        Subg_UART3_Rx_Data[Rx_DMA_BuffNum] = '\0'; /* 添加字符串结束符 */

        
        ZigBee_Recvice_Flag = 1;  //接收标志位

        /* 重新设置DMA传输 */
        dma_memory_address_config(DMA0,DMA_CH2,DMA_MEMORY_0,(uint32_t)UART3_Rx_DMA_buffer);
        dma_transfer_number_config(DMA0,DMA_CH2,sizeof(UART3_Rx_DMA_buffer));
        dma_channel_enable(DMA0, DMA_CH2);  /* 开启DMA传输 */
    }
    
}


