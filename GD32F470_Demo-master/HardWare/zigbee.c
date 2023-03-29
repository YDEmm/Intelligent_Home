#include "zigbee.h"

static void  Gpio_Init( void )
{	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);					  	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_7 ; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  						
	GPIO_Init(GPIOB,&GPIO_InitStructure);           		   				 
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 								
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  					
	GPIO_Init(GPIOB,&GPIO_InitStructure);           		  	  		
		
}
/*  usart2 ��ʼ��------PA2  PA3*/
static void USART2_Init(u32 bound)
{
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//TX			   //�������PA2
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* ��ʼ����������IO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//RX			 //��������PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //ģ������
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* ��ʼ��GPIO */
	
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���1 
	USART_ClearFlag(USART2, USART_FLAG_TC);
		
	USART_ITConfig(USART2, USART_IT_RXNE,ENABLE);//��������ж�
	//USART_ITConfig(USART3, USART_FLAG_IDLE, ENABLE);//��������ж�	
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
}
static void Usart2_SendString(const char *str)
{
	while(*str !='\0')
		{						
			while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE)); //�ж��Ƿ���Է���
			USART_SendData(USART2,*str);
			while(!USART_GetFlagStatus(USART2, USART_FLAG_TC)); //�ж��Ƿ�����ɣ��˾�����У��������
			str++;													//��ֻ�������һ���ַ������ǣ�
		}
}
/*
 * ��������zigbee_Cmd
 * ����  ����zigbeeģ�鷢��ATָ��
 * ����  ��cmd�������͵�ָ��
 *         reply1��reply2���ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ
 *         waittime���ȴ���Ӧ��ʱ��
 * ����  : 1��ָ��ͳɹ�
 *         0��ָ���ʧ��
 * ����  �����ⲿ����
 */
static int CC2530_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime )
{    
	static int i = 0;
	Zigbee_Rec.Rec_Timeflag = waittime; //��ʼ�����볬ʱʱ��

	Zigbee_Rec.Rec_End = RECNOEND;

	Zigbee_Rec.Wait_Time = 0;
	Zigbee_Rec.Wait_Flag = 1; // �����ȴ���ʱ	
	
	Usart2_SendString(cmd);	
	while(Zigbee_Rec.Rec_End == waittime)
	{
		if(Zigbee_Rec.Wait_Time > 500)
		{
			i++;
			Zigbee_Rec.Wait_Time = 0;
			Zigbee_Rec.Wait_Flag = 0;
			usb_printf("��ʱ\r\n");
			return FAIL;
		}		
	}
	if(strstr(reply1,"NULL") == NULL  && strstr(reply1,"NULL") == NULL)
	{
		Zigbee_Rec.Rec_String[Zigbee_Rec.Rec_Num] = '\0';
		usb_printf("CC2530_Cmd: %s\r\n",Zigbee_Rec.Rec_String);	
		if(strstr(Zigbee_Rec.Rec_String,reply1) != NULL || strstr(Zigbee_Rec.Rec_String,reply2) != NULL)
		{
			memset(Zigbee_Rec.Rec_String,0,RX_BUF_MAX_LEN);  	//����ַ��� 
			Zigbee_Rec.Rec_Num = 0;						
			return TRUE;		
		}
		else 
		{
			memset(Zigbee_Rec.Rec_String,0,RX_BUF_MAX_LEN);		//����ַ��� 
			Zigbee_Rec.Rec_Num = 0;	
			return FAIL;
		}									
		
	}
	else
	{
		memset(Zigbee_Rec.Rec_String,0,RX_BUF_MAX_LEN);		//����ַ��� 
		Zigbee_Rec.Rec_Num = 0;			
		return 2;
	}
}

/*
 * ��������CC2530_DEV
 * ����  ��ѡ��CC2530ģ��Ľڵ�����
 * ����  ��enumMode���ڵ�����
 * ����  : 1��ѡ��ɹ�
 *         0��ѡ��ʧ��
 * ����  �����ⲿ����
 */
static int CC2530_DEV ( char mode )
{
	switch ( mode )
	{
		case 'C':
			return CC2530_Cmd ( "AT+DEV=C", "+OK", NULL,500 ); 
		
        case 'R':
		  return CC2530_Cmd ( "AT+DEV=R", "+OK", NULL, 500 ); 
		
		case 'E':
		  return CC2530_Cmd ( "AT+DEV=E", "+OK", NULL, 500 ); 
		
        default:
		  return FAIL;
  }
	
}
/*
 * ��������CC2530_Mode
 * ����  ��ѡ��CC2530ģ��Ĺ���ģʽ
 * ����  ��Mode������ģʽ
 * ����  : 1��ѡ��ɹ�
 *         0��ѡ��ʧ��
 * ����  �����ⲿ����
 */
static int CC2530_Mode (char mode)
{
	switch (mode)
	{
		case 'C':
			return CC2530_Cmd ( "AT+MODE=1", "+OK", NULL, 500 ); 
		
	  case 'B':
		  return CC2530_Cmd ( "AT+MODE=2", "+OK", NULL, 500 ); 
		
		case 'X':
		  return CC2530_Cmd ( "AT+MODE=3", "+OK", NULL, 500 ); 
		
	  default:
		  return FAIL;
  }
	
}

static int CC2530_IN_AT ( void )
{
	 return  CC2530_Cmd ( "+++", "+OK", NULL, 500 );   	
}

static int CC2530_EXIT_AT ( void )
{
	 return  CC2530_Cmd ( "AT+EXIT", "+OK", NULL, 500 );   	
}

static int CC2530_GROUP ( void )
{
	 return  CC2530_Cmd ( "AT+GROUP=15", "+OK", NULL, 500 );   	
}

static int CC2530_CH (void)
{

	 return CC2530_Cmd ( "AT+CH=12", "+OK", NULL, 500 );   	

}

static int CC2530_PANID ( void )
{
	 return CC2530_Cmd ( "AT+PANID=FFFF", "+OK", NULL, 500 );   	
}

/*
 * ��������CC2530_RESET
 * ����  ������CC2530ģ��
 * ����  ����
 * ����  : ��
 * ����  ��
 */
static int CC2530_RESET ( void )
{

	 return CC2530_Cmd ( "AT+RESET", "+OK", NULL, 1000 );   	

}



Uart Zigbee_Rec = {0};

/***************
* 	�������ƣ�Zigbee_send()
*	�������ܣ�ͨ��Zigbee͸��ģʽ�����ַ���
	�βΣ�str
	����ֵ����
***************/
void Zigbee_send(char *str)
{
	Usart2_SendString(str);
}

/***************
* 	�������ƣ�Zigbee_recive()
*	�������ܣ�ͨ��Zigbee͸��ģʽ�����ַ���
	�βΣ���
	����ֵ��str
***************/
char* Zigbee_recive(void)
{
	static char *Str = NULL;

	if(Zigbee_Rec.Rec_End == RECEND)
	{
		if(strstr(Zigbee_Rec.Rec_String,"END") == 0)
		{			
			memcpy(Str,Zigbee_Rec.Rec_String,strlen(Zigbee_Rec.Rec_String));
			memset(Zigbee_Rec.Rec_String,0,strlen(Zigbee_Rec.Rec_String));
			Zigbee_Rec.Rec_Num = 0;			
			return Str;
		}
		else
		{
			return NULL;
		}

	}
	else
	{
		return NULL;
	}
	
}


void USART2_IRQHandler(void)
{	
	char Rec_buf;
	//char clear;
		
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET)
	{			
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);			
		
		Zigbee_Rec.Rec_Time = 0;
		Zigbee_Rec.Wait_Time = 0;
		Zigbee_Rec.Wait_Flag = 0;
		
		Zigbee_Rec.Rec_Start = RECSTART; 				// ��ʼ��������
		Zigbee_Rec.Rec_End   = RECNOEND;
				
		Rec_buf  = USART_ReceiveData(USART2);			
		Zigbee_Rec.Rec_String[Zigbee_Rec.Rec_Num++] = Rec_buf; 	//��Ž����ַ���
		//usb_printf("%c\r\n",Rec_buf); //
		//Zigbee_Rec.Rec_String[Zigbee_Rec.Rec_Num] = '\0'; 										//��ճ�ʱ����ʱ��		
		if(strstr(Zigbee_Rec.Rec_String,"START") == 0)
		{
			Zigbee_Rec.Rec_Start = RECSTART; 				// ��ʼ��������
			Zigbee_Rec.Rec_End   = RECNOEND;
		}
		else if(strstr(Zigbee_Rec.Rec_String,"END") == 0)
		{
			Zigbee_Rec.Rec_End   = RECEND;
			Zigbee_Rec.Rec_Start = RECNOSTART;
			Zigbee_Rec.Rec_Time = 0; //���ʱ��
		}
		if(Zigbee_Rec.Rec_Num > RX_BUF_MAX_LEN )
		{
			Zigbee_Rec.Rec_Num = 0;
			usb_printf("ERROR_WORDNUM IS FULL\r\n");
		}
					
	}
}



void Test(void)
{	

	SETAT();
    while(!CC2530_DEV('C'));
    usb_printf("C\r\n");
  
	while(!CC2530_Mode('C'));
    usb_printf("TC\r\n");
		
    while(!CC2530_GROUP());
    usb_printf("GROUP\r\n");
    
	while(!CC2530_CH());
    usb_printf("CH\r\n");
    
	while(!CC2530_PANID());
    usb_printf("PANID\r\n");
	
    SETHEX();
	char *STR = NULL;
	while(1)
	{
		delay_ms(1000);
		Usart2_SendString("ddddd\r\n");
		//if((STR = Zigbee_recive())!= NULL)
		//{
		//	usb_printf("%s\r\n",STR);
		//}
		
	}	
}

//ZIGBEE ��ʼ��
void Zigbee_Init(void)
{
	int times = 0;
	Gpio_Init();
	USART2_Init(115200);
	//Lcd_Cmd("page zigbeeInit");
	//Lcd_Cmd("page zigbeeInit");
//	while (1)
//	{
//		times++;
//		SETAT();   //����ΪATģʽ/
//			
//		while(CC2530_DEV('C') == 0)
//		{
//		}
//		//Lcd_Cmd("t0.txt=\"Э����ģʽ������..\"");
//		//Lcd_Cmd("j0.val=10");
//		usb_printf("C\r\n");
//		while(CC2530_Mode('C') == 0)
//		{
//		}
//		//Lcd_Cmd("t1.txt=\"ģʽ1������..\"");
//		//Lcd_Cmd("j0.val=20");
//		usb_printf("TC\r\n");

//		while(CC2530_GROUP() == 0)
//		{
//		}
//		//Lcd_Cmd("t2.txt=\"�������������..\"");
//		//Lcd_Cmd("j0.val=35");
//		usb_printf("GROUP\r\n");
//		
//		while(CC2530_CH() == 0)
//		{
//		}	
//		//Lcd_Cmd("t3.txt=\"��������ͨ����..\"");
//		//Lcd_Cmd("j0.val=55");
//		usb_printf("CC2530_CH\r\n");
//		while(CC2530_PANID() == 0)
//		{
//		}
//		//Lcd_Cmd("t4.txt=\"CC2530_PANID..\"");
//		usb_printf("CC2530_PANID\r\n");
//		//Lcd_Cmd("j0.val=65");
//		
//		CC2530_RESET();
//		//Lcd_Cmd("j0.val=75");
//		//Lcd_Cmd("t5.txt=\"CC2530_RESET..\"");
//		usb_printf("CC2530_RESET\r\n");
//		delay_ms(200);
//		if(NET == 0 || times == 8)
//		{
//			if(times == 8)
//			{
//				usb_printf("δ��������\r\n");
//				//Lcd_Cmd("t6.txt=\"��������ʧ�ܣ����zigbee\"");
//			}
//			usb_printf("�Ѽ�������\r\n");
//			//Lcd_Cmd("t6.txt=\"�Ѽ�������\"");
//			break;
//		}
//		
//	}	
//		//Lcd_Cmd("j0.val=90");
//		usb_printf("�Ѽ�������\r\n");
//		SETHEX();
//		//Lcd_Cmd("t7.txt=\"������Ϊ͸��ģʽ\"");
//		delay_ms(300);
//		//Lcd_Cmd("j0.val=100");
//		//Lcd_Cmd("page main");
}
