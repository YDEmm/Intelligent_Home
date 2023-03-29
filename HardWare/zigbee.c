#include "zigbee.h"
#include "function.h"
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
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2; 		    //�������PA2
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);               //��ʼ����������IO
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;         	//��������PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; //ģ������
	GPIO_Init(GPIOA,&GPIO_InitStructure);               //��ʼ��GPIO
	
	USART_InitStructure.USART_BaudRate = bound;         //����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2 
	USART_ClearFlag(USART2, USART_FLAG_TC);
		
	USART_ITConfig(USART2, USART_IT_RXNE,ENABLE);//���������ж�
	USART_ITConfig(USART2, USART_FLAG_IDLE, ENABLE);//���������ж�	
	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;       //����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                        //����ָ���Ĳ�����ʼ��VIC�Ĵ���	
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

/***************
* 	�������ƣ�Zigbee_send()
*	�������ܣ�ͨ��Zigbee͸��ģʽ�����ַ���
	�βΣ�str
	����ֵ����
***************/
Uart Zigbee_Rec = {0};
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
* 	�������ƣ�Zigbee_Read()
*	�������ܣ�Zigbee��ȡ
	�βΣ�waittime �ȴ�ʱ�� char recstr �ַ���������
	����ֵ��1 �ɹ�   0 ʧ��
    ���ã����ⲿ����
***************/
int Zigbee_Read(int waittime,char recstr[])
{
	Zigbee_Rec.Rec_Timeflag = waittime;  //���ó�ʱ�ȴ�ʱ��
    
//    if(Zigbee_Rec.Rec_String[0] == 'S')
//    {
//        Zigbee_Rec.Rec_Start = RECSTART; 				// ��ʼ��������
//        Zigbee_Rec.Rec_Finsh = RECNOEND;
//    }
//    if(Zigbee_Rec.Rec_String[Zigbee_Rec.Rec_Num] == 'E')
//    {
//        Zigbee_Rec.Rec_Finsh = RECEND;
//        Zigbee_Rec.Rec_Start = RECNOSTART;
//        Zigbee_Rec.Rec_Time = 0; //���ʱ��
//        Zigbee_Rec.Rec_Num = 0;
//    }
    
	if((Zigbee_Rec.Rec_Start == 0) && (Zigbee_Rec.Rec_Finsh == 1)) //�ȴ��������
	{      	
		//usb_printf("FLAG:%s str:%d Num:%d\r\n",Zigbee_Rec.Rec_String,strlen(Zigbee_Rec.Rec_String),Zigbee_Rec.Rec_Num);
		
		memcpy(recstr,Zigbee_Rec.Rec_String,sizeof(Zigbee_Rec.Rec_String));//�����յ������ݿ������µ�ַ
		
		memset(Zigbee_Rec.Rec_String,0,sizeof(Zigbee_Rec.Rec_String));

		Zigbee_Rec.Rec_Num = 0;
		Zigbee_Rec.Rec_Start = 0;
		Zigbee_Rec.Rec_Time = 0;
		Zigbee_Rec.Rec_Finsh = 0;
		return 1;//�������
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
            //��Ž����ַ���
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
//            Zigbee_Rec.Rec_Time = 0; //���ʱ��
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
        Zigbee_Rec.Rec_Time = 0; //���ʱ��
        Zigbee_Rec.Rec_Num = 0;       
		Zigbee_Rec.Wait_Time = 0;
		Zigbee_Rec.Wait_Flag = 0;
    }
}

//ZIGBEE ��ʼ��
u8 ZigBee_Send_Flag = 0;
u8 ZigBee_Recevice_Flag = 0;
void Zigbee_Init(void)
{
    ZigBee_Send_Flag = 1;
    ZigBee_Recevice_Flag = 1;
	USART2_Init(115200);
}
