#include "zigbee.h"
#include "function.h"
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
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2; 		    //串口输出PA2
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);               //初始化串口输入IO
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;         	//串口输入PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; //模拟输入
	GPIO_Init(GPIOA,&GPIO_InitStructure);               //初始化GPIO
	
	USART_InitStructure.USART_BaudRate = bound;         //波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口2
	
	USART_Cmd(USART2, ENABLE);  //使能串口2 
	USART_ClearFlag(USART2, USART_FLAG_TC);
		
	USART_ITConfig(USART2, USART_IT_RXNE,ENABLE);//开启接收中断
	USART_ITConfig(USART2, USART_FLAG_IDLE, ENABLE);//开启空闲中断	
	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;       //串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                        //根据指定的参数初始化VIC寄存器	
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

/***************
* 	函数名称：Zigbee_send()
*	函数功能：通过Zigbee透传模式发送字符串
	形参：str
	返回值：无
***************/
Uart Zigbee_Rec = {0};
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

	if(Zigbee_Rec.Rec_Finsh == RECEND)
	{
		if(Zigbee_Rec.Rec_String[Zigbee_Rec.Rec_Num] == 'E')
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
/***************
* 	函数名称：Zigbee_Read()
*	函数功能：Zigbee读取
	形参：waittime 等待时间 char recstr 字符串缓冲区
	返回值：1 成功   0 失败
    调用：被外部调用
***************/
int Zigbee_Read(int waittime,char recstr[])
{
	Zigbee_Rec.Rec_Timeflag = waittime;  //设置超时等待时间
    
//    if(Zigbee_Rec.Rec_String[0] == 'S')
//    {
//        Zigbee_Rec.Rec_Start = RECSTART; 				// 开始接收数据
//        Zigbee_Rec.Rec_Finsh = RECNOEND;
//    }
//    if(Zigbee_Rec.Rec_String[Zigbee_Rec.Rec_Num] == 'E')
//    {
//        Zigbee_Rec.Rec_Finsh = RECEND;
//        Zigbee_Rec.Rec_Start = RECNOSTART;
//        Zigbee_Rec.Rec_Time = 0; //清空时间
//        Zigbee_Rec.Rec_Num = 0;
//    }
    
	if((Zigbee_Rec.Rec_Start == 0) && (Zigbee_Rec.Rec_Finsh == 1)) //等待接收完成
	{      	
		//usb_printf("FLAG:%s str:%d Num:%d\r\n",Zigbee_Rec.Rec_String,strlen(Zigbee_Rec.Rec_String),Zigbee_Rec.Rec_Num);
		
		memcpy(recstr,Zigbee_Rec.Rec_String,sizeof(Zigbee_Rec.Rec_String));//将接收到的数据拷贝到新地址
		
		memset(Zigbee_Rec.Rec_String,0,sizeof(Zigbee_Rec.Rec_String));

		Zigbee_Rec.Rec_Num = 0;
		Zigbee_Rec.Rec_Start = 0;
		Zigbee_Rec.Rec_Time = 0;
		Zigbee_Rec.Rec_Finsh = 0;
		return 1;//接收完成
	}
	return 0;
}

void USART2_IRQHandler(void)
{	
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET)
	{			
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
        ZigBee_Recevice_Flag = 1;        
        Zigbee_Rec.Rec_Start = RECSTART; 				
        Zigbee_Rec.Rec_Finsh = RECNOEND;
        if(Zigbee_Rec.Rec_Num <= RX_BUF_MAX_LEN )
        {
            //存放接收字符串
            Zigbee_Rec.Rec_String[Zigbee_Rec.Rec_Num++] = USART_ReceiveData(USART2);
        }
        //Zigbee_Rec.Rec_String[Zigbee_Rec.Rec_Num] = '\0';		
//        if(Zigbee_Rec.Rec_String[0] == 'S')
//        {
//            Zigbee_Rec.Rec_Start = RECSTART; 				
//            Zigbee_Rec.Rec_Finsh = RECNOEND;
//        }
//        if(Zigbee_Rec.Rec_String[Zigbee_Rec.Rec_Num] == 'E')
//        {
//            Zigbee_Rec.Rec_Finsh = RECEND;
//            Zigbee_Rec.Rec_Start = RECNOSTART;
//            Zigbee_Rec.Rec_Time = 0; //清空时间
//            Zigbee_Rec.Rec_Num = 0;
//        }   
         else if(Zigbee_Rec.Rec_Num > RX_BUF_MAX_LEN )
        {
            Zigbee_Rec.Rec_Num = 0;
            printf("ERROR_WORDNUM IS FULL\r\n");
        }
    }    
    else if(USART_GetITStatus(USART2,USART_IT_IDLE)!= RESET)
    {
        USART_ClearITPendingBit(USART2,USART_IT_IDLE);
        USART_ReceiveData(USART2);
        ZigBee_Recevice_Flag = 0; 
        Zigbee_Rec.Rec_Finsh = RECEND;
        Zigbee_Rec.Rec_Start = RECNOSTART;
        Zigbee_Rec.Rec_Time = 0; //清空时间
        Zigbee_Rec.Rec_Num = 0;       
		Zigbee_Rec.Wait_Time = 0;
		Zigbee_Rec.Wait_Flag = 0;
    }
}

//ZIGBEE 初始化
u8 ZigBee_Send_Flag = 0;
u8 ZigBee_Recevice_Flag = 0;
void Zigbee_Init(void)
{
    ZigBee_Send_Flag = 1;
    ZigBee_Recevice_Flag = 1;
	USART2_Init(115200);
}
