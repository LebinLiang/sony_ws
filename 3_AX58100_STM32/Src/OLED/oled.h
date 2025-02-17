#ifndef __OLED_H
#define __OLED_H			  	 
#include "stm32f4xx_hal.h"
#include "stdlib.h"	    
#include "System.h"


#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
 

//OLEDģʽ����
//0: 4�ߴ���ģʽ  ��ģ���BS1��BS2����GND��
//1: ����8080ģʽ ��ģ���BS1��BS2����VCC��
#define OLED_MODE 	0 
		    						  
//-----------------OLED�˿ڶ���----------------  					   
#define OLED_CS 	PGout(3)//CS
#define OLED_RST  PGout(5)//RES
#define OLED_RS 	PGout(4)//DC
#define OLED_WR 	PGout(0)//  
#define OLED_RD 	PGout(1)
 
//ʹ��4�ߴ��нӿ�ʱʹ�� 
#define OLED_SCLK 	PGout(7)
#define OLED_SDIN 	PGout(6)
		     
#define OLED_CMD  	0		//д����
#define OLED_DATA 	1		//д����
//OLED�����ú���
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		


							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);	 
#endif  
	 



