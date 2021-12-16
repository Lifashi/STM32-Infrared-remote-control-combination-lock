#ifndef _BSP_OLED_H
#define _BSP_OLED_H

#include "stm32f10x.h"
#include "stdio.h"
#include "bsp_spi.h"
#include "bsp_delay.h"
#include "bsp_flash.h"

#define OLED_WIGH 128  //128列
#define OLED_PAGE 8	 // 一页8行 8*8 64行 

#define  OLED_GPIO_RES_PORT			GPIOA
#define  OLED_GPIO_RES_PIN			GPIO_Pin_4
#define  OLED_GPIO_RES_CLK			RCC_APB2Periph_GPIOA
#define  OLED_RCC_APBxClockCmd		RCC_APB2PeriphClockCmd

#define  OLED_GPIO_DC_PORT			GPIOA
#define  OLED_GPIO_DC_PIN			GPIO_Pin_15
#define  OLED_GPIO_DC_CLK			RCC_APB2Periph_GPIOA


#define OLED_DC_H		GPIO_SetBits(OLED_GPIO_DC_PORT,OLED_GPIO_DC_PIN);
#define OLED_DC_L		GPIO_ResetBits(OLED_GPIO_DC_PORT,OLED_GPIO_DC_PIN);			

#define OLED_RES_H		GPIO_SetBits(OLED_GPIO_RES_PORT,OLED_GPIO_RES_PIN);
#define OLED_RES_L		GPIO_ResetBits(OLED_GPIO_RES_PORT,OLED_GPIO_RES_PIN);			

#define DEBUG 1
#define FLASH_INFO(fmt,arg...)  printf("<<FLASH-INFO>> "fmt"\n",##arg);
#define FLASH_ERROR(fmt,arg...) printf("<<FLASH-ERROR>> "fmt"\n",##arg);
#define FLASH_DEBUG(fmt,arg...) do{\
                                 if(DEBUG)\
                                 printf("<<EEPROM-DEBUG>> FILE[%s] LINE[%d] DATE[%s]\n"fmt"",__FILE__,__LINE__,__DATE__ ##arg);\
                                 }while(0) //__FILE__ (两个下划线！！！) 调用如下：FLASH_DEBUG();
extern const unsigned char ascii_8_16[95][16];
								 
void OLED_Init(void);
void OLED_SetPos(u8 x,u8 y);
void OLED_Clear(void);
void OLED_SendByte_Data(u8 data);
void OLED_WriteChar(u8 x,u8 y,u8 ch);	
void OLED_WriteString(u8 x,u8 y,u8 *str);
void OLED_Write_Number(u8 x,u8 y,u8 num);							
void OLED_Write_A_Number(u8 x,u8 y,u16 num);
void OLED_Write_A_Chinaese(u8 x,u8 y,u8 *ch);	
void OLED_Write_Chinaese(u8 x,u8 y,u8 *ch);							 
#endif
