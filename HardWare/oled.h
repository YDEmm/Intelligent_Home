#ifndef __OLED_H
#define __OLED_H 

#include "sys.h"
#include "stdlib.h"	

//-----------------OLED端口定义---------------- 

#define OLED_SCL_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_13)//SCL
#define OLED_SCL_Set() GPIO_SetBits(GPIOB,GPIO_Pin_13)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_15)//SDA
#define OLED_SDA_Set() GPIO_SetBits(GPIOB,GPIO_Pin_15)

#define OLED_RES_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_12)//RES
#define OLED_RES_Set() GPIO_SetBits(GPIOA,GPIO_Pin_12)

#define OLED_DC_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_11)//DC
#define OLED_DC_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_11)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_15)


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

#define Menu_level 5    //菜单级数
#define Menu_Length 20  //单个菜单长度

typedef struct
{
    uint8_t string[20]; //菜单名
    void (*func) (void); //菜单函数
}GUI_Menu;   //OLED
typedef struct Oled_Gui
{
    uint8_t Pause;   //Gui暂停标志位
    uint8_t Menu_Len; //Gui菜单长度
    uint8_t Begin_Index;  //Gui菜单起始
    uint8_t Now_Inedx;  //Gui菜单选中
    GUI_Menu Menu[Menu_Length];  //Gui菜单
    void (*Last_Func[Menu_level]) (void);    //Gui上级函数记录
    void (*Now_Func) (void);   //Gui选中函数
}Gui;
extern Gui My_Gui;



void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
void OLED_ShowSerialChinese(u16 x,u16 y,char *str,u8 size1,u8 mode);
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
void OLED_Init(void);

#endif

