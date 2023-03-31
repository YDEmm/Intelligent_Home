#include "UART4_Lcd.h"
#include "ZigBee_UART3.h"

static rcu_periph_enum COM_CLK[COMn] = {LCD_COM0_CLK};
static uint32_t COM_TX_PIN[COMn] = {LCD_COM0_TX_PIN};
static uint32_t COM_RX_PIN[COMn] = {LCD_COM0_RX_PIN};

Lcd_uart Lcd_Rec = {0};

void UART4_Lcd_Init(uint32_t com)
{
    /* enable GPIO clock */
    uint32_t COM_ID = 0;
    if(LCD_COM0 == com)
    {
        COM_ID = 0U;
    }
    
    rcu_periph_clock_enable( LCD_COM0_TX_CLK);
    rcu_periph_clock_enable( LCD_COM0_RX_CLK);
    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[COM_ID]);
    /* connect port to USARTx_Tx */
    gpio_af_set(LCD_COM0_TX_PORT, LCD_COM0_AF, COM_TX_PIN[COM_ID]);
    /* connect port to USARTx_Rx */
    gpio_af_set(LCD_COM0_RX_PORT, LCD_COM0_AF, COM_RX_PIN[COM_ID]);
    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(LCD_COM0_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_TX_PIN[COM_ID]);
    gpio_output_options_set(LCD_COM0_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_TX_PIN[COM_ID]);
    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(LCD_COM0_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_RX_PIN[COM_ID]);
    gpio_output_options_set(LCD_COM0_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_RX_PIN[COM_ID]);

    /* USART configure */
    usart_deinit(LCD_COM0);
    usart_baudrate_set(LCD_COM0,115200U);                    //设置波特率
    usart_parity_config(LCD_COM0,USART_PM_NONE);             //无奇偶校验
    usart_word_length_set(LCD_COM0,USART_WL_8BIT);           //8位数据位
    usart_stop_bit_set(LCD_COM0,USART_STB_1BIT);             //1位停止位
    usart_receive_config(LCD_COM0, USART_RECEIVE_ENABLE);    //使能串口接收
    usart_transmit_config(LCD_COM0, USART_TRANSMIT_ENABLE);  //使能串口发送
    usart_enable(LCD_COM0);                                  //使能串口4
    
    /* com init config */
    nvic_irq_enable(UART4_IRQn,0,0);
    
    usart_interrupt_flag_clear(UART4,USART_INT_FLAG_TC); /* 清除空闲中断标志位 */
    usart_interrupt_enable(LCD_COM0, USART_INT_RBNE);
     
}
/* 串口发送一个字节 */
static void UART4_Send_Byte(uint8_t ch)
{
    //发送一个字节数据到USART0
    usart_data_transmit(UART4,ch);
    
    //等待发送完毕
    while(usart_flag_get(UART4,USART_FLAG_TBE) == RESET);
    
}

/* 串口发送字符串，直到遇到字符穿结束符 */
static void UART4_Send_String(uint8_t *str)
{
    unsigned int k = 0;
    do
    {
        UART4_Send_Byte(*(str + k));
        k++;
    }while(*(str + k) != '\0');
}


void UART4_IRQHandler(void)
{
    if(usart_flag_get(LCD_COM0, USART_FLAG_RBNE) != RESET)   //接收中断
    {
        usart_interrupt_flag_clear(LCD_COM0, USART_INT_FLAG_RBNE);
        Lcd_Rec.Rec_Time = 0;     //清空时间
		if(Lcd_Rec.Rec_Num == 0)
		{
			Lcd_Rec.Rec_Start = 1; //开始接收数据 第一帧
		}	
		if(Lcd_Rec.Rec_Num < RX_BUF_MAX_LEN)	
		{
			Lcd_Rec.Rec_String[Lcd_Rec.Rec_Num++] = usart_data_receive(UART4); //读取字符
		}
    }
}

void Lcd_Cmd(char *buf)
{
	uint8_t CmdTxBuf[128] = {0};
	char Rec_buf[128] = {0};
	
	strcpy((char *)Rec_buf,buf);
	strcpy((char *)CmdTxBuf,buf);
				
	for(int i=0;i<strlen(Rec_buf)+3;i++)
	{
		if(i>=strlen(Rec_buf))
		{
			CmdTxBuf[i] = 0xff;
		}
	}
	UART4_Send_String(CmdTxBuf);	
}

int Lcd_Read(int waittime,char str[])
{
	Lcd_Rec.Rec_Timeflag = waittime;  //设置超时等待时间
	if(Lcd_Rec.Rec_Finsh == 1 || (strstr(Lcd_Rec.Rec_String,"END") != NULL)) //等待接收完成
	{      		
		// while (Lcd_Rec.Rec_String[i++] != '\0')
		// {
		// 	/* code */
		// }
			
		printf("%2x %2x %2x %2x %2x %2x %2x \r\n",Lcd_Rec.Rec_String[0],
										Lcd_Rec.Rec_String[1],
										Lcd_Rec.Rec_String[2],
										Lcd_Rec.Rec_String[3],
										Lcd_Rec.Rec_String[4],
										Lcd_Rec.Rec_String[5],
										Lcd_Rec.Rec_String[6]);
		
		printf("%s\r\n",Lcd_Rec.Rec_String);
		strcpy(str,Lcd_Rec.Rec_String);
		memset(Lcd_Rec.Rec_String,0,sizeof(Lcd_Rec.Rec_String));
		Lcd_Rec.Rec_Num = 0;
		Lcd_Rec.Rec_Start = 0;
		Lcd_Rec.Rec_Time = 0;
		Lcd_Rec.Rec_Finsh = 0;
		return 1;
	}
	return 0;
	//接收完成
}







