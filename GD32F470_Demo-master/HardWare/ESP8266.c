#include "ESP8266.h"
#include "systick.h"
#include <string.h>
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
    //发送一个字节数据到USART6
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
        data =  usart_data_receive(UART6);
        if(data)
        {
            if(ESP8266_Buff.UART6_Index < 510)
            { 
                ESP8266_Buff.UART6_Buff[ESP8266_Buff.UART6_Index++] = data;
            }
        }
        else
        {
            ESP8266_Buff.UART6_Buff[ESP8266_Buff.UART6_Index++] = ' ';
        }
       
    }
    else if(usart_flag_get(ESP8266_COM0, USART_FLAG_IDLE) != RESET) //空闲中断
    {
        usart_interrupt_flag_clear(ESP8266_COM0,USART_INT_FLAG_IDLE); /* 清除空闲中断标志位 */
        usart_data_receive(UART6);
        ESP8266_Buff.UART6_Buff[ESP8266_Buff.UART6_Index] = '\0';
        ESP8266_Buff.UART6_Finish = 1;
    }
}

/**
 * 
 * [ESP8266_SendCmd 发送命令到ESP8266]
 * @param  cmd   [需要发送的AT指令]
 * @param  reply [期望模块回显的内容]
 * @param  wait  [等待的时间(ms)]
 * @return       [期望内容等于实际回显内容返回1，否则0]
 * 
 */
int ESP8266_SendCmd(char* cmd, char* reply, int wait)
{
    ESP8266_Buff.UART6_Index = 0;
    printf("[ESP8266_SendCmd] %s\r\n", cmd);

    Send_ESP8266_String((uint8_t*)cmd);

    delay_1ms(wait);

    if (strcmp(reply, "") == 0)
    {
        return 0;
    }

    if (ESP8266_Buff.UART6_Index != 0 && ESP8266_Buff.UART6_Finish == 1)
    {
        //ESP8266_Buff.UART6_Finish = 0;    //标志位清空在每个连接函数的后面 不能在此处清空-----否则后面无法判断
        ESP8266_Buff.UART6_Buff[ESP8266_Buff.UART6_Index] = '\0';

        if (strstr(ESP8266_Buff.UART6_Buff, reply))
        {
            printf("\r\n%s+++YES\r\n", ESP8266_Buff.UART6_Buff);

            return 1;
        }
        else if (strstr(ESP8266_Buff.UART6_Buff, "busy"))
        {
            printf("busy...\r\n");

            delay_1ms(3000);

            return 0;
        }
        else
        {  
            printf("\r\n%s+++NO\r\n", ESP8266_Buff.UART6_Buff);

            return 0;
        }  
    }  
}

/**
 * [ESP8266_Init 模块初始化]
 * @return [成功为1，否则为0]
 */
int ESP8266_Init(void)  
{
    int ret = 0;
    ret = ESP8266_SendCmd("AT+CWMODE=3\r\n","OK\r\n", 1000);

    ESP8266_SendCmd("AT+RST\r\n","OK\r\n", 1000);  //重启
    ESP8266_SendCmd("AT+CWMODE?\r\n","OK\r\n",1000);//查看本机配置模式

    if (!ret)
    {
        printf("Cannot initialize ESP module\r\n");
        return 0;
    }
    return ret;
}

/*
****************************************************************************************
* Function: ESP32ConnetWIFI
* Description: ESP8266连接网络
* Input: ssid：指向目标AP的SSID（WIFI名）   pwd：指向WIFI密码
* Output: None
* Return: 0：成功连接
* Author: weihaoMo
* Others: WiFi模组工作模式为单STA模式
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
uint8_t ESP32ConnetWIFI(uint8_t *ssid,uint8_t *pwd)
{
	memset(ESP8266_Buff.UART6_Buff,0,sizeof(ESP8266_Buff.UART6_Buff));
	uint8_t buf[100]={"AT+CWJAP="};
	
	//ESP8266_SendCmd("AT+CWQAP\r\n","OK\r\n",1000);
	
	strcat((char *)buf,"\"");
	strcat((char *)buf,(const char *)ssid);
	strcat((char *)buf,"\",\"");
	strcat((char *)buf,(const char *)pwd);
	strcat((char *)buf,"\"\r\n");
	ESP8266_SendCmd((char *)buf,"OK\r\n",1000);	
	//Send_ESP8266_String(buf);
	
	while(1)
	{	
		while(!ESP8266_Buff.UART6_Finish);//等待传输完成标志
		ESP8266_Buff.UART6_Finish=0;//清标志
				
		if(NULL!=strstr((const char *)ESP8266_Buff.UART6_Buff,(const char *)"OK\r\n"))
		{
			printf("------------------WIFI-------------------\r\n");
//			uart1_send_string((uint8_t *)w_rev_buf_copy);
			printf("ESP32 WIFI CONNECT OK\r\n");	
			//OLED_CLear(0);			
			return 0;
		}
	}
}

/*
****************************************************************************************
* Function: ESP32ConnetServer
* Description: ESP8266连接服务器
* Input: type:连接类型（TCP/UDP） remote_ip:服务器IP   remote_port：服务器端口
* Output: None
* Return: 0：成功连接
* Author: weihaoMo
* Others: WiFi模组工作模式为单STA模式
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
uint8_t ESP32ConnetServer(uint8_t* tppe,uint8_t *remote_ip,uint8_t *remote_port)
{
    memset(ESP8266_Buff.UART6_Buff,0,sizeof(ESP8266_Buff.UART6_Buff));
	uint8_t buf[100]={"AT+CIPSTART="};
	strcat((char *)buf,"\"");
	strcat((char *)buf,(const char *)tppe);
	strcat((char *)buf,"\",\"");
	strcat((char *)buf,(const char *)remote_ip);
	strcat((char *)buf,"\",");
	strcat((char *)buf,(const char *)remote_port);
	strcat((char *)buf,"\r\n");
	
	ESP8266_SendCmd((char *)buf,"OK\r\n",1000);	
	//Send_ESP8266_String(buf);
	
	while(1)
	{
		while(!ESP8266_Buff.UART6_Finish);//等待传输完成标志
		ESP8266_Buff.UART6_Finish = 0;//清标志
				
		if(NULL!=strstr((const char *)ESP8266_Buff.UART6_Buff,(const char *)"OK"))
		{
            printf("------------------TCP-------------------\r\n");
			printf("ESP32 Server CONNECT OK\r\n");			
			return 0;
		}

	}
}

/*
****************************************************************************************
* Function: ExitPassThroughMode
* Description: 退出透传模式
* Input: None
* Output: None
* Return: None
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
void ExitPassThroughMode(void)
{
	Send_ESP8266_String((uint8_t *)"+++");
	/*发送 +++ 退出透传时，请至少间隔 1秒再发下一条 AT 指令*/
	delay_1ms(1000);
}

/*
****************************************************************************************
* Function: ESP32Restore
* Description: 恢复出厂设置，擦除所有保存在flash的参数，恢复默认参数，设备重启
* Input: None
* Output: None
* Return: None
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
uint8_t ESP32Restore(void)
{

	Send_ESP8266_String((uint8_t *)"AT+RESTORE\r\n");

	while(1)
	{
		while(!ESP8266_Buff.UART6_Finish);//等待传输完成标志
		ESP8266_Buff.UART6_Finish=0;//清标志
				
		if(NULL!=strstr((const char *)ESP8266_Buff.UART6_Buff,(const char *)"ready"))
		{
//			printf("-------------------------------------\r\n");
//			uart1_send_string((uint8_t *)w_rev_buf_copy);
			printf("ESP32 RESTORE OK\r\n");		
			return 0;
		}
	}
	
}

/*
****************************************************************************************
* Function: ESP8266TcpInit
* Description: ESP32网络初始化
* Input: None
* Output: None
* Return: None
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
uint8_t ESP8266TcpInit(void)
{
	uint8_t ret;
	ExitPassThroughMode( );//退出透传
	
	ESP32Restore( );//复位，恢复出厂设置
	delay_1ms(500);
	
	ret = ESP8266_SendCmd("AT+GMR\r\n","OK\r\n",1000);//查询版本信息
	if(!ret)
	{
		printf("AT+GMR error\r\n");
		return 2;
	}
	
	ret = ESP8266_SendCmd("AT+CWMODE=1\r\n","OK\r\n",1000);//配置WiFi模组工作模式为单STA模式，并把配置保存在flash
	if(!ret)
	{
		printf("AT+CWMODE error\r\n");
		return 3;
	}

 
//	ret = ESP32SendATcmd((uint8_t *)"AT+CWAUTOCONN=1\r\n",(uint8_t *)"OK");//使能上电自动连接AP
//	if(ret)
//	{
//		printf("3 no this respond\r\n");
//		return 3;
//	}
	
	ret = ESP8266_SendCmd("AT+CIPMODE=1\r\n","OK",1000);//设置透传
	if(!ret)
	{
		printf("AT+CIPMODE=1 error\r\n");
		return 4;
	}
	
	
	printf("TCP Set OK\r\n");
	return ret;
}
