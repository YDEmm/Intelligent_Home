#include "function.h"

char Zigbee_Readstr[128] = {0};
char Zigbee_Sendstr[128] = {0};

ThresHod Auto_Threshod;//阈值
void AUTO_Threshod_Init(void)   //初始阈值设置
{
    
    Auto_Threshod.Temperature_Min = 10;
    Auto_Threshod.Temperature_Max = 30;
    Auto_Threshod.Humidity_Min = 20;
    Auto_Threshod.Humidity_Max = 65;
    Auto_Threshod.Intensity_Min = 20;
    Auto_Threshod.Intensity_Max = 80;
    Auto_Threshod.Smoke_Min = 20;
    Auto_Threshod.Smoke_Max = 1000;
    Auto_Threshod.Air = 0;
    Auto_Threshod.Curtain = 0;
    Auto_Threshod.Window = 0;
}
void LED_Install(void)
{
        gd_eval_led_on(LED1);
        gd_eval_led_off(LED2);
        delay_1ms(1000);
        gd_eval_led_on(LED2);
        gd_eval_led_off(LED1);
        delay_1ms(1000);
}

/* 心跳机制发送 */
void Get_Val(int Auto_flag,ThresHod Auto_Threshod)
{
    if((Zibgbee_sendtime > 400) && (Run_flag != ZIGBEECONFIG) &&
       (Run_flag != SETZIGBEE) && (Run_flag != AUTOOFFHANDON) && (Run_flag != SET))   
    {
        if(Auto_flag == AUTOON)    //自动模式------将初始的阈值拼接起来通过ZigBee发送给子节点实现子节点自动模式
        {
            sprintf(UART3_Tx_DMA_buffer,  "S-""ON""-T:%d+%d""-H:%d+%d""-I:%d+%d""-S:%d+%d""-E\r\n"                                                          
                                    ,Auto_Threshod.Temperature_Min
                                    ,Auto_Threshod.Temperature_Max
                                    ,Auto_Threshod.Humidity_Min
                                    ,Auto_Threshod.Humidity_Max
                                    ,Auto_Threshod.Intensity_Min
                                    ,Auto_Threshod.Intensity_Max
                                    ,Auto_Threshod.Smoke_Min
                                    ,Auto_Threshod.Smoke_Max);
            
        }
        else if(Auto_flag == AUTOOFF)//手动模式------将设备状态通过子节点发送给子节点实现手动控制
        {
            memset(UART3_Tx_DMA_buffer,0,256);
            sprintf(UART3_Tx_DMA_buffer,  "S-""OFF""-A:%d""-C:%d""-W:%d""-LED1:%d""-LED2:%d""-E\r\n"                                                         
                                    ,Auto_Threshod.Air
                                    ,Auto_Threshod.Curtain
                                    ,Auto_Threshod.Window
                                    ,Auto_Threshod.LED1
                                    ,Auto_Threshod.LED2);            
        }
        UART3_DMA_Send(UART3_Tx_DMA_buffer);
        printf("主机发送的控制语句：%s\r\n",UART3_Tx_DMA_buffer);
        Zibgbee_sendtime = 0;
    }
}

char Lcd_reccompare[][16] = {"AUTOON",
                            "AUTOOFF",
                            "AUTOOFFHANDON",
                            "HANDNUM",
                            "THRESHOLD_SET",
                            "SETZIGBEE",
                            "ZIGBEECONFIG",
                            "SET"};

int Run_flag; //全局变量 操作屏幕使用

int UART_Lcd_Task(ThresHod *Auto_Threshod)
{
    char LCD_Rec[64] = {0};
    int find_i = 0, find_j = 0;
    
    if(Lcd_Read(15,LCD_Rec) == 1)    //超时接收串口屏发送的信息
    {
        while(strstr(LCD_Rec,Lcd_reccompare[find_i++])==NULL)
		{	
			if(strstr(LCD_Rec,Lcd_reccompare[find_j--])!=NULL)
			{
				find_i = find_j+2;//检测到后j为多减一次的 所以加一 再次加1 抵消while
				break;
			}
			if (find_i == find_j)
			{
				return USELESS;
			}
			
		}
		--find_i;
		switch(find_i)
		{
			case AUTOON:                     //自动模式-----------
				printf("case:AUTOON\r\n");
			//    Zigbee_send(Lcd_rec);      //手动模式 发送手动模式指令
				Run_flag=AUTOON;
				return  AUTOON;				
			case AUTOOFF:
				printf("case:AUTOOFF\r\n");
			//    Zigbee_send(Lcd_rec);      //手动模式 发送手动模式指令
				Run_flag=AUTOOFF;
				return AUTOOFF;
			case AUTOOFFHANDON:
				printf("case:AUTOOFHANDON\r\n");
				
			//    Zigbee_send(Lcd_rec);      //手动模式 发送手动模式指令
				Run_flag=AUTOOFFHANDON;
				return AUTOOFFHANDON;
			case HANDNUM:
				printf("case:HANDNUM:%s\r\n",LCD_Rec);   
				sscanf(LCD_Rec,"HANDNUM-%d-%d-%d-%d-%d",
                                &Auto_Threshod->Air,
                                &Auto_Threshod->Curtain,
                                &Auto_Threshod->Window,
                                &Auto_Threshod->LED1,
                                &Auto_Threshod->LED2);                
			 //   Zigbee_send(Lcd_rec);      //手动模式 发送手动模式指令
				Run_flag=USELESS;
				return USELESS;
			case THRESHOLD_SET:
				printf("case:THRESHOLD_SET:%s\r\n",LCD_Rec);
				sscanf(LCD_Rec,"THRESHOLD_SET-%d+%d-%d+%d-%d+%d-%d+%d",
                                                     &Auto_Threshod->Temperature_Min,
                                                     &Auto_Threshod->Temperature_Max,
                                                     &Auto_Threshod->Humidity_Min,
                                                     &Auto_Threshod->Humidity_Max,
                                                     &Auto_Threshod->Intensity_Min,
                                                     &Auto_Threshod->Intensity_Max,
													 &Auto_Threshod->Smoke_Min,
													 &Auto_Threshod->Smoke_Max);
				Run_flag=USELESS;
				return USELESS;
			case SETZIGBEE:
				printf("case:SETZIGBEE\r\n");
				Run_flag=SETZIGBEE;
				return SETZIGBEE;
			case ZIGBEECONFIG:
				printf("case:ZIGBEECONFIG\r\n");
				Run_flag=ZIGBEECONFIG;
				return ZIGBEECONFIG;
			case SET:
				printf("case:SET\r\n");
				Run_flag=SET;
				return SET;
			default:
				Run_flag=USELESS;
				return USELESS;
		}		
	}
     return USELESS;
}
//改变运行模式
int Change_mode(int Run_mode,int *Auto_flag)
{
    if(Run_mode == AUTOON)
	{	
		printf("Change_mode AUTOON\r\n");
        *Auto_flag = AUTOON;
	}
    else if(Run_mode == AUTOOFF)
    {
        printf("Change_mode AUTOOFF\r\n");
        *Auto_flag = AUTOOFF;	
    }
    return 0;
}

void ESP8266_Send_Recevice(void)
{
    int OStime=0;
    OStime++;
    delay_1ms(1);
    if(OStime%1000==0)
    {
        Send_ESP8266_String("AT\r\n");
    }
    if(ESP8266_Buff.UART6_Finish)
    {
        printf("%s\n",ESP8266_Buff.UART6_Buff);
        memset(ESP8266_Buff.UART6_Buff,0,sizeof(ESP8266_Buff.UART6_Buff));
        ESP8266_Buff.UART6_Index = 0;
        ESP8266_Buff.UART6_Finish = 0;
    }
    
}

void IrDA_Send_Recevice(ThresHod *Auto_Threshod,IrDA_Type *IrDA_Buff)
{
    if(IrDA_Buff->USART5_Finish)
    {
        printf("%c",IrDA_Buff->data);
        if(IrDA_Buff->data == 0x43)  //切换温湿度
        {
            switch(IrDA_Buff->Flag)//-----------选择项目（T(min/max)  H(min/max)  I(min/max)  S(min/max)）
            {
                case 0: *IrDA_Buff->Value_ThresHod = &Auto_Threshod->Temperature_Min;  break;   
                case 1: *IrDA_Buff->Value_ThresHod = &Auto_Threshod->Temperature_Max;  break;   
                case 2: *IrDA_Buff->Value_ThresHod = &Auto_Threshod->Humidity_Min;     break;   
                case 3: *IrDA_Buff->Value_ThresHod = &Auto_Threshod->Humidity_Max;     break;   
                case 4: *IrDA_Buff->Value_ThresHod = &Auto_Threshod->Intensity_Min;    break;   
                case 5: *IrDA_Buff->Value_ThresHod = &Auto_Threshod->Intensity_Max;    break;   
                case 6: *IrDA_Buff->Value_ThresHod = &Auto_Threshod->Smoke_Min;        break;   
                case 7: *IrDA_Buff->Value_ThresHod = &Auto_Threshod->Smoke_Max;        break;   
            }
            IrDA_Buff->Flag += 1;
        }
            
        switch(IrDA_Buff->data)
        {
            case 0x16:   break;    //0 -----------设置阈值
            case 0x0C:   break;    //1
            case 0x18:   break;    //2
            case 0x5E:   break;    //3
            case 0x08:   break;    //4
            case 0x1C:   break;    //5
            case 0x5A:   break;    //6
            case 0x42:   break;    //7
            case 0x52:   break;    //8
            case 0x4A:   break;    //9
            case 0x07:   break; **(IrDA_Buff->Value_ThresHod) --;  printf("%d\r\n",**IrDA_Buff->Value_ThresHod);//- -----------控制阈值加减
            case 0x15:   break;    //+
            case 0x09:   break;    //ok ----------确认
            case 0x45:   break;    //ch- ---------Air
            case 0x46:   break;    //ch  ---------Cur
            case 0x47:   break;    //ch+ ---------Win
        }
        IrDA_Buff->USART5_Finish = 0;
    }
    
}
    





