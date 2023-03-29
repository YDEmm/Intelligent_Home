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
/*  usart2 初始化------PA2  PA3*/
static void USART2_Init(u32 bound)
{
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//TX			   //串口输出PA2
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* 初始化串口输入IO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//RX			 //串口输入PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //模拟输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* 初始化GPIO */
	
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART2, ENABLE);  //使能串口1 
	USART_ClearFlag(USART2, USART_FLAG_TC);
		
	USART_ITConfig(USART2, USART_IT_RXNE,ENABLE);//开启相关中断
	//USART_ITConfig(USART3, USART_FLAG_IDLE, ENABLE);//开启相关中断	
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}
static void Usart2_SendString(const char *str)
{
	while(*str !='\0')
		{						
			while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE)); //判断是否可以发送
			USART_SendData(USART2,*str);
			while(!USART_GetFlagStatus(USART2, USART_FLAG_TC)); //判断是否发送完成，此句必须有，否则会造
			str++;													//成只发送最后一个字符（覆盖）
		}
}
/*
 * 函数名：zigbee_Cmd
 * 描述  ：对zigbee模块发送AT指令
 * 输入  ：cmd，待发送的指令
 *         reply1，reply2，期待的响应，为NULL表不需响应，两者为或逻辑关系
 *         waittime，等待响应的时间
 * 返回  : 1，指令发送成功
 *         0，指令发送失败
 * 调用  ：被外部调用
 */
static int CC2530_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime )
{    
	static int i = 0;
	Zigbee_Rec.Rec_Timeflag = waittime; //初始化理想超时时间

	Zigbee_Rec.Rec_End = RECNOEND;

	Zigbee_Rec.Wait_Time = 0;
	Zigbee_Rec.Wait_Flag = 1; // 开启等待超时	
	
	Usart2_SendString(cmd);	
	while(Zigbee_Rec.Rec_End == waittime)
	{
		if(Zigbee_Rec.Wait_Time > 500)
		{
			i++;
			Zigbee_Rec.Wait_Time = 0;
			Zigbee_Rec.Wait_Flag = 0;
			usb_printf("超时\r\n");
			return FAIL;
		}		
	}
	if(strstr(reply1,"NULL") == NULL  && strstr(reply1,"NULL") == NULL)
	{
		Zigbee_Rec.Rec_String[Zigbee_Rec.Rec_Num] = '\0';
		usb_printf("CC2530_Cmd: %s\r\n",Zigbee_Rec.Rec_String);	
		if(strstr(Zigbee_Rec.Rec_String,reply1) != NULL || strstr(Zigbee_Rec.Rec_String,reply2) != NULL)
		{
			memset(Zigbee_Rec.Rec_String,0,RX_BUF_MAX_LEN);  	//清空字符串 
			Zigbee_Rec.Rec_Num = 0;						
			return TRUE;		
		}
		else 
		{
			memset(Zigbee_Rec.Rec_String,0,RX_BUF_MAX_LEN);		//清空字符串 
			Zigbee_Rec.Rec_Num = 0;	
			return FAIL;
		}									
		
	}
	else
	{
		memset(Zigbee_Rec.Rec_String,0,RX_BUF_MAX_LEN);		//清空字符串 
		Zigbee_Rec.Rec_Num = 0;			
		return 2;
	}
}

/*
 * 函数名：CC2530_DEV
 * 描述  ：选择CC2530模块的节点类型
 * 输入  ：enumMode，节点类型
 * 返回  : 1，选择成功
 *         0，选择失败
 * 调用  ：被外部调用
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
 * 函数名：CC2530_Mode
 * 描述  ：选择CC2530模块的工作模式
 * 输入  ：Mode，工作模式
 * 返回  : 1，选择成功
 *         0，选择失败
 * 调用  ：被外部调用
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
 * 函数名：CC2530_RESET
 * 描述  ：重启CC2530模块
 * 输入  ：无
 * 返回  : 无
 * 调用  ：
 */
static int CC2530_RESET ( void )
{

	 return CC2530_Cmd ( "AT+RESET", "+OK", NULL, 1000 );   	

}



Uart Zigbee_Rec = {0};

/***************
* 	函数名称：Zigbee_send()
*	函数功能：通过Zigbee透传模式发送字符串
	形参：str
	返回值：无
***************/
void Zigbee_send(char *str)
{
	Usart2_SendString(str);
}

/***************
* 	函数名称：Zigbee_recive()
*	函数功能：通过Zigbee透传模式接收字符串
	形参：无
	返回值：str
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
		
		Zigbee_Rec.Rec_Start = RECSTART; 				// 开始接收数据
		Zigbee_Rec.Rec_End   = RECNOEND;
				
		Rec_buf  = USART_ReceiveData(USART2);			
		Zigbee_Rec.Rec_String[Zigbee_Rec.Rec_Num++] = Rec_buf; 	//存放接收字符串
		//usb_printf("%c\r\n",Rec_buf); //
		//Zigbee_Rec.Rec_String[Zigbee_Rec.Rec_Num] = '\0'; 										//清空超时接收时间		
		if(strstr(Zigbee_Rec.Rec_String,"START") == 0)
		{
			Zigbee_Rec.Rec_Start = RECSTART; 				// 开始接收数据
			Zigbee_Rec.Rec_End   = RECNOEND;
		}
		else if(strstr(Zigbee_Rec.Rec_String,"END") == 0)
		{
			Zigbee_Rec.Rec_End   = RECEND;
			Zigbee_Rec.Rec_Start = RECNOSTART;
			Zigbee_Rec.Rec_Time = 0; //清空时间
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

//ZIGBEE 初始化
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
//		SETAT();   //配置为AT模式/
//			
//		while(CC2530_DEV('C') == 0)
//		{
//		}
//		//Lcd_Cmd("t0.txt=\"协调器模式配置中..\"");
//		//Lcd_Cmd("j0.val=10");
//		usb_printf("C\r\n");
//		while(CC2530_Mode('C') == 0)
//		{
//		}
//		//Lcd_Cmd("t1.txt=\"模式1配置中..\"");
//		//Lcd_Cmd("j0.val=20");
//		usb_printf("TC\r\n");

//		while(CC2530_GROUP() == 0)
//		{
//		}
//		//Lcd_Cmd("t2.txt=\"分配网络分组中..\"");
//		//Lcd_Cmd("j0.val=35");
//		usb_printf("GROUP\r\n");
//		
//		while(CC2530_CH() == 0)
//		{
//		}	
//		//Lcd_Cmd("t3.txt=\"分配网络通道中..\"");
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
//				usb_printf("未加入网络\r\n");
//				//Lcd_Cmd("t6.txt=\"加入网络失败，检测zigbee\"");
//			}
//			usb_printf("已加入网络\r\n");
//			//Lcd_Cmd("t6.txt=\"已加入网络\"");
//			break;
//		}
//		
//	}	
//		//Lcd_Cmd("j0.val=90");
//		usb_printf("已加入网络\r\n");
//		SETHEX();
//		//Lcd_Cmd("t7.txt=\"已配置为透传模式\"");
//		delay_ms(300);
//		//Lcd_Cmd("j0.val=100");
//		//Lcd_Cmd("page main");
}
