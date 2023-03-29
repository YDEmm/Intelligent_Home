/*
 * @Author: JYM
 * @Date: 2023-01-25 21:14:37
 * @LastEditTime: 2023-01-25 21:53:30
 * @LastEditors: JYM
 * @Description: 
 * @FilePath: \JYM--DHT11\User\Src\function.c
 * 
 */
#include "function.h"
#include "bmp.h"

ThresHold Auto_Threshod;          //��ֵ
char Zigbee_Readstr[128] = {0};
char Zigbee_Sendstr[128] = {0};
void ZigBee_Send_Recevice(void)
{
    static int Auto_Flag = 0;         //�л��Զ����ֶ���־λ
    u8 Temperature = 0, Humidity = 0; //��ʪ�� 
    u16 Intensity = 0;                //����ǿ��
    float Adc_MQ_2 = 0.0;             //��������Ũ��
    Adc_MQ_2 = MQ2_GetPPM();          //��ȡ����Ũ��
    Intensity = BH1750_Read_Data(SlaveAddress);//��ȡ����ǿ��
    //Zigbee_Readstr = Zigbee_recive();
    
     if(DHT11_Read_Data(&Temperature,NULL,&Humidity,NULL) != 0 )    //��ȡ��ʪ��
    {    
        
    }
    
    if(Zigbee_Read(20,Zigbee_Readstr) != 0)         //Zigbee��ʱ����
    {
        if(strstr(Zigbee_Readstr,"ON") != NULL)     //�Զ�ģʽ ������ֵ
        {
            Auto_Flag = 0;            
            sscanf(Zigbee_Readstr,  "S-""ON""-T:%d+%d""-H:%d+%d""-I:%d+%d""-S:%d+%d""-E\r\n"                                                          
                                    ,&Auto_Threshod.Temperature_Min
                                    ,&Auto_Threshod.Temperature_Max
                                    ,&Auto_Threshod.Humidity_Min
                                    ,&Auto_Threshod.Humidity_Max
                                    ,&Auto_Threshod.Intensity_Min
                                    ,&Auto_Threshod.Intensity_Max
                                    ,&Auto_Threshod.Smoke_Min
                                    ,&Auto_Threshod.Smoke_Max);
            
        }
        else                                        //�ֶ�ģʽ  �ֶ����ƿյ�����������������  OFF-A:%d-C:%d-W:%d\r\n
        {
            Auto_Flag = 1;
            sscanf(Zigbee_Readstr,  "S-""OFF""-A:%d""-C:%d""-W:%d""-LED1:%d""-LED2:%d""-E\r\n"                                                          
                                    ,&Auto_Threshod.Air
                                    ,&Auto_Threshod.Curtain
                                    ,&Auto_Threshod.Window
                                    ,&Auto_Threshod.LED1
                                    ,&Auto_Threshod.LED2);
            
            //usb_printf(Zigbee_Readstr);
            
            RELAY1 = Auto_Threshod.Air;             //�յ����ƿ���
            RELAY2 = Auto_Threshod.Window;          //�������ƿ���
            if(Auto_Threshod.Curtain == 1)  Step_Motor_LOOP(1,20,1);  //���������ת--������
            else if(Auto_Threshod.Curtain == 0)  Step_Motor_LOOP(0,20,1);  //���������ת--������           
        }
    }
    if(strstr(Zigbee_Rec.Rec_String,"ON") != NULL) 
    {
        OLED_Mode_Flag = 1;
    }
    if(strstr(Zigbee_Rec.Rec_String,"OFF") != NULL) 
    {
        OLED_Mode_Flag = 2;
    }
    //��Ҫ���������-----�����������ݡ�����ֹͣ��������
    
    sprintf(Zigbee_Sendstr,"Temperature-%d��    Humidity-%dRH    Intensity-%dLux    Smoke-%.2fppm",Temperature,Humidity,Intensity,Adc_MQ_2);
    if(ZigBee_Send_Flag == 1)
    {
        ZigBee_Send_Flag = 0;             //zigbee���ͱ�־λ  �ö�ʱ��2s����һ��
        Zigbee_send(Zigbee_Sendstr);
    }
    if(ZigBee_Recevice_Flag == 1)
    {
        printf("���������͵��ӽڵ���Ϣ: %s\r\n",Zigbee_Sendstr);
        printf("��ȡ���������͵���Ϣ:%s \r\n",Zigbee_Readstr);
        printf("��ȡ���������͵��ֽ�����%d\r\n",Zigbee_Rec.Rec_Num);
        printf("\r\n");   //����
    }
    //printf("smoke:%d ppm\n", Get_Adc_Average(ADC_Channel_1,30));//���ģ����   1---4096   0---0  
}

/**
 * @description: ����������Ժ���
 * @return {*}
 */
void Step_Motor_Test(void)
{
    /*
    ������һ����������������ת 1����ת   0����ת
    �ڶ������������Ƶ��ת��Ȧ��
    ��������������ʱ*/

    Step_Motor_LOOP(1,20,1);  //�����������

    
//    Step_Motor_NUM(1,100,1); 
//    delay_ms(500);
}

int OLED_Mode_Flag;    //OLEDģʽ��־λ--0����ʼ����-1���ӽڵ���Ϣ-2������ģʽ��״̬
int Clear_Flag = 0;    //������־
char OLED_T_Buff[16];
char OLED_H_Buff[16];
char OLED_I_Buff[16];
char OLED_S_Buff[16];
char OLED_A_Buff[16];
char OLED_C_Buff[16];
char OLED_W_Buff[16];
char OLED_LED_Buff[16];
u8 OLED_Clear_Flag = 0;
void OLED_Task(void)
{
    OLED_Refresh();
    
    memset(OLED_T_Buff,0,sizeof(OLED_T_Buff));
    memset(OLED_H_Buff,0,sizeof(OLED_T_Buff));
    memset(OLED_I_Buff,0,sizeof(OLED_T_Buff));
    memset(OLED_S_Buff,0,sizeof(OLED_T_Buff));
    memset(OLED_A_Buff,0,sizeof(OLED_T_Buff));
    memset(OLED_C_Buff,0,sizeof(OLED_T_Buff));
    memset(OLED_W_Buff,0,sizeof(OLED_T_Buff));
    memset(OLED_LED_Buff,0,sizeof(OLED_T_Buff));
    switch(OLED_Mode_Flag)
    {
        case 0:                                     //�˵�
        {
            OLED_ShowPicture(0,0,128,64,BMP2,1);
            delay_ms(2000);
            OLED_ShowString(0,10,(u8 *)"--------------------",8,1);
            OLED_ShowChinese(0,20,3,16,1);//��
            OLED_ShowChinese(20,20,4,16,1);//��
            OLED_ShowChinese(40,20,5,16,1);//��
            OLED_ShowChinese(60,20,6,16,1);//��
            OLED_ShowChinese(80,20,7,16,1);//��
            OLED_ShowChinese(100,20,8,16,1);//��

            OLED_ShowString(0,100,(u8 *)"--------------------",8,1);
            OLED_ShowString(0,110,(u8 *)"* * * * * * * * j y m",8,1);
        }break;
        case 1:                                     //AUTOON
        {
            sprintf(OLED_T_Buff,"Temp:%d+%d",Auto_Threshod.Temperature_Min,Auto_Threshod.Temperature_Max);
            sprintf(OLED_H_Buff,"Humi:%d+%d",Auto_Threshod.Humidity_Min,Auto_Threshod.Humidity_Max);
            sprintf(OLED_I_Buff,"Inte:%d+%d",Auto_Threshod.Intensity_Min,Auto_Threshod.Intensity_Max);
            sprintf(OLED_S_Buff,"Smok:%d+%d",Auto_Threshod.Smoke_Min,Auto_Threshod.Smoke_Max);

            OLED_ShowString(0,0,(u8 *)"-------AUTOON-------",8,1);
            OLED_ShowString(0,10,(u8 *)OLED_T_Buff,8,1);
            OLED_ShowString(0,20,(u8 *)OLED_H_Buff,8,1);
            OLED_ShowString(0,30,(u8 *)OLED_I_Buff,8,1);
            OLED_ShowString(0,40,(u8 *)OLED_S_Buff,8,1);
            OLED_ShowString(0,50,(u8 *)"--------------------",8,1);
        }break;
        case 2:                                     //AUTOOFF
        {
            sprintf(OLED_A_Buff,"Air-----%d",Auto_Threshod.Air);
            sprintf(OLED_C_Buff,"Cur-----%d",Auto_Threshod.Curtain);
            sprintf(OLED_W_Buff,"Win-----%d",Auto_Threshod.Window);
            sprintf(OLED_LED_Buff,"LED1--%d LED2--%d",Auto_Threshod.LED1,Auto_Threshod.LED2);

            OLED_ShowString(0,0,(u8 *)"-------AUTOOFF------",8,1);
            OLED_ShowString(0,10,(u8 *)OLED_A_Buff,8,1);
            OLED_ShowString(0,20,(u8 *)OLED_C_Buff,8,1);
            OLED_ShowString(0,30,(u8 *)OLED_W_Buff,8,1);
            OLED_ShowString(0,40,(u8 *)OLED_LED_Buff,8,1);
            OLED_ShowString(0,50,(u8 *)"--------------------",8,1);
        }break;
    }
}





