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
    usart_baudrate_set(ESP8266_COM0,115200U);                    //���ò�����
    usart_parity_config(ESP8266_COM0,USART_PM_NONE);             //����żУ��
    usart_word_length_set(ESP8266_COM0,USART_WL_8BIT);           //8λ����λ
    usart_stop_bit_set(ESP8266_COM0,USART_STB_1BIT);             //1λֹͣλ
    usart_receive_config(ESP8266_COM0, USART_RECEIVE_ENABLE);    //ʹ�ܴ��ڽ���
    usart_transmit_config(ESP8266_COM0, USART_TRANSMIT_ENABLE);  //ʹ�ܴ��ڷ���
    
    
    /* com init config */
    nvic_irq_enable(UART6_IRQn,0,0);
    
    usart_interrupt_flag_clear(UART6,USART_INT_FLAG_TC); /* ��������жϱ�־λ */
    usart_interrupt_enable(ESP8266_COM0, USART_INT_RBNE);
    usart_interrupt_enable(ESP8266_COM0, USART_INT_IDLE);
    usart_enable(ESP8266_COM0);                                  //ʹ�ܴ���4
     
}
/* ���ڷ����ַ��� */
void Send_ESP8266_String(uint8_t *str)
{
    //����һ���ֽ����ݵ�USART6
    while(*str != '\0')
    {
        usart_data_transmit(UART6,*str++);
        
        //�ȴ��������
        while(usart_flag_get(UART6,USART_FLAG_TBE) == RESET);
    }
}

ESP8266_Type ESP8266_Buff; 
void UART6_IRQHandler(void)
{
    uint8_t data;
    if(usart_flag_get(ESP8266_COM0, USART_FLAG_RBNE) != RESET)// &&(!usart6finish))   //�����ж�
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
    else if(usart_flag_get(ESP8266_COM0, USART_FLAG_IDLE) != RESET) //�����ж�
    {
        usart_interrupt_flag_clear(ESP8266_COM0,USART_INT_FLAG_IDLE); /* ��������жϱ�־λ */
        usart_data_receive(UART6);
        ESP8266_Buff.UART6_Buff[ESP8266_Buff.UART6_Index] = '\0';
        ESP8266_Buff.UART6_Finish = 1;
    }
}

/**
 * 
 * [ESP8266_SendCmd �������ESP8266]
 * @param  cmd   [��Ҫ���͵�ATָ��]
 * @param  reply [����ģ����Ե�����]
 * @param  wait  [�ȴ���ʱ��(ms)]
 * @return       [�������ݵ���ʵ�ʻ������ݷ���1������0]
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
        //ESP8266_Buff.UART6_Finish = 0;    //��־λ�����ÿ�����Ӻ����ĺ��� �����ڴ˴����-----��������޷��ж�
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
 * [ESP8266_Init ģ���ʼ��]
 * @return [�ɹ�Ϊ1������Ϊ0]
 */
int ESP8266_Init(void)  
{
    int ret = 0;
    ret = ESP8266_SendCmd("AT+CWMODE=3\r\n","OK\r\n", 1000);

    ESP8266_SendCmd("AT+RST\r\n","OK\r\n", 1000);  //����
    ESP8266_SendCmd("AT+CWMODE?\r\n","OK\r\n",1000);//�鿴��������ģʽ

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
* Description: ESP8266��������
* Input: ssid��ָ��Ŀ��AP��SSID��WIFI����   pwd��ָ��WIFI����
* Output: None
* Return: 0���ɹ�����
* Author: weihaoMo
* Others: WiFiģ�鹤��ģʽΪ��STAģʽ
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
		while(!ESP8266_Buff.UART6_Finish);//�ȴ�������ɱ�־
		ESP8266_Buff.UART6_Finish=0;//���־
				
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
* Description: ESP8266���ӷ�����
* Input: type:�������ͣ�TCP/UDP�� remote_ip:������IP   remote_port���������˿�
* Output: None
* Return: 0���ɹ�����
* Author: weihaoMo
* Others: WiFiģ�鹤��ģʽΪ��STAģʽ
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
		while(!ESP8266_Buff.UART6_Finish);//�ȴ�������ɱ�־
		ESP8266_Buff.UART6_Finish = 0;//���־
				
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
* Description: �˳�͸��ģʽ
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
	/*���� +++ �˳�͸��ʱ�������ټ�� 1���ٷ���һ�� AT ָ��*/
	delay_1ms(1000);
}

/*
****************************************************************************************
* Function: ESP32Restore
* Description: �ָ��������ã��������б�����flash�Ĳ������ָ�Ĭ�ϲ������豸����
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
		while(!ESP8266_Buff.UART6_Finish);//�ȴ�������ɱ�־
		ESP8266_Buff.UART6_Finish=0;//���־
				
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
* Description: ESP32�����ʼ��
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
	ExitPassThroughMode( );//�˳�͸��
	
	ESP32Restore( );//��λ���ָ���������
	delay_1ms(500);
	
	ret = ESP8266_SendCmd("AT+GMR\r\n","OK\r\n",1000);//��ѯ�汾��Ϣ
	if(!ret)
	{
		printf("AT+GMR error\r\n");
		return 2;
	}
	
	ret = ESP8266_SendCmd("AT+CWMODE=1\r\n","OK\r\n",1000);//����WiFiģ�鹤��ģʽΪ��STAģʽ���������ñ�����flash
	if(!ret)
	{
		printf("AT+CWMODE error\r\n");
		return 3;
	}

 
//	ret = ESP32SendATcmd((uint8_t *)"AT+CWAUTOCONN=1\r\n",(uint8_t *)"OK");//ʹ���ϵ��Զ�����AP
//	if(ret)
//	{
//		printf("3 no this respond\r\n");
//		return 3;
//	}
	
	ret = ESP8266_SendCmd("AT+CIPMODE=1\r\n","OK",1000);//����͸��
	if(!ret)
	{
		printf("AT+CIPMODE=1 error\r\n");
		return 4;
	}
	
	
	printf("TCP Set OK\r\n");
	return ret;
}
