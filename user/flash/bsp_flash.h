#ifndef _BSP_SPI_FLASH_H
#define _BSP_SPI_FLASH_H

#include "stm32f10x.h"
#include "stdio.h"
#include "bsp_spi.h"



#define DEBUG 1
#define FLASH_INFO(fmt,arg...)  printf("<<FLASH-INFO>> "fmt"\n",##arg);
#define FLASH_ERROR(fmt,arg...) printf("<<FLASH-ERROR>> "fmt"\n",##arg);
#define FLASH_DEBUG(fmt,arg...) do{\
                                 if(DEBUG)\
                                 printf("<<EEPROM-DEBUG>> FILE[%s] LINE[%d] DATE[%s]\n"fmt"",__FILE__,__LINE__,__DATE__ ##arg);\
                                 }while(0) //__FILE__ (两个下划线！！！) 调用如下：FLASH_DEBUG();

#define FLASH_PageByte  256
#define FLASH_TimeOut	1000	
								 
#define DEMMY				0X00  //值随便
#define W25Q16_ID			0X9F
#define Write_Enable		0X06
#define Read_Status			0X05
#define Sector_Erase_4		0X20
#define Block_Erase_32		0X52
#define Block_Erase_64		0XD8
#define Read_Data			0X03
#define Write_Data			0X02
#define Power_Down			0XB9								 
#define Release_Power_Down  0XAB
								 
void FLASH_Init(void);
u32 SPI_FLASH_ReadID(void);
void  FLASH_Power_Downr_Enable(void);
u8  FLASH_Power_Downr_Disable(void);
void SPI_FLASH_Erase(u32 addr);
void SPI_FLASH_WriteNum(u32 addr ,u32 data);
void SPI_FLASH_ReadNum(u32 addr,u8 *Num ,u8 NumByte);
void SPI_FLASH_ReadData(u32 addr ,u8 *data,u32 numbyte);
void SPI_FLASH_WriteData(u32 addr ,u8 *data,u32 numbyte);
void SPI_FLASH_ReadPageData(u32 addr ,u8 *data,u32 numbyte);
void SPI_FLASH_Write_BufferData(u32 addr ,u8 *data,u32 WriteNumByte);
#endif
