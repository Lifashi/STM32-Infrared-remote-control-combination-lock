#ifndef _BSP_SPI_H
#define _BSP_SPI_H

#include "stm32f10x.h"
#include "stdio.h"

#if 0  //SPI1  CS---PB0
#define  SPI_GPIO_SCK_PORT			GPIOA
#define  SPI_GPIO_SCK_PIN			GPIO_Pin_5
#define  SPI_GPIO_SCK_CLK			RCC_APB2Periph_GPIOA
#define  GPIO_RCC_APBxClockCmd		RCC_APB2PeriphClockCmd

#define  SPI_GPIO_MISO_PORT			GPIOA
#define  SPI_GPIO_MISO_PIN			GPIO_Pin_6
#define  SPI_GPIO_MISO_CLK			RCC_APB2Periph_GPIOA

#define  SPI_GPIO_MOSI_PORT			GPIOA
#define  SPI_GPIO_MOSI_PIN			GPIO_Pin_7
#define  SPI_GPIO_MOSI_CLK			RCC_APB2Periph_GPIOA

#define  SPI_GPIO_CS_PORT			GPIOB
#define  SPI_GPIO_CS_PIN			GPIO_Pin_0
#define  SPI_GPIO_CS_CLK			RCC_APB2Periph_GPIOB

#define SPIx						SPI1
#define SPIx_CLK					RCC_APB2Periph_SPI1
#define	SPIx_RCC_APBxClockCmd		RCC_APB2PeriphClockCmd

#elif 1
// SPI1  CS---PB0
#define  SPI1_GPIO_SCK_PORT			GPIOA
#define  SPI1_GPIO_SCK_PIN			GPIO_Pin_5
#define  SPI1_GPIO_SCK_CLK			RCC_APB2Periph_GPIOA
#define  SPI1_GPIO_APBxClockCmd		RCC_APB2PeriphClockCmd

#define  SPI1_GPIO_MISO_PORT			GPIOA
#define  SPI1_GPIO_MISO_PIN			GPIO_Pin_6
#define  SPI1_GPIO_MISO_CLK			RCC_APB2Periph_GPIOA

#define  SPI1_GPIO_MOSI_PORT			GPIOA
#define  SPI1_GPIO_MOSI_PIN			GPIO_Pin_7
#define  SPI1_GPIO_MOSI_CLK			RCC_APB2Periph_GPIOA

#define  SPI1_GPIO_CS_PORT			GPIOB
#define  SPI1_GPIO_CS_PIN			GPIO_Pin_0
#define  SPI1_GPIO_CS_CLK			RCC_APB2Periph_GPIOB

#define SPI_1						SPI1
#define SPI1_CLK					RCC_APB2Periph_SPI1
#define	SPI1_RCC_APBxClockCmd		RCC_APB2PeriphClockCmd

#define SPI1_CS_H		GPIO_SetBits(SPI1_GPIO_CS_PORT,SPI1_GPIO_CS_PIN);
#define SPI1_CS_L		GPIO_ResetBits(SPI1_GPIO_CS_PORT,SPI1_GPIO_CS_PIN)

//SPI2  CS---PB7
#define  SPI2_GPIO_SCK_PORT			GPIOB
#define  SPI2_GPIO_SCK_PIN			GPIO_Pin_13
#define  SPI2_GPIO_SCK_CLK			RCC_APB2Periph_GPIOB
#define  SPI2_GPIO_APBxClockCmd		RCC_APB2PeriphClockCmd

#define  SPI2_GPIO_MISO_PORT		GPIOB
#define  SPI2_GPIO_MISO_PIN			GPIO_Pin_14
#define  SPI2_GPIO_MISO_CLK			RCC_APB2Periph_GPIOB

#define  SPI2_GPIO_MOSI_PORT		GPIOB
#define  SPI2_GPIO_MOSI_PIN			GPIO_Pin_15
#define  SPI2_GPIO_MOSI_CLK			RCC_APB2Periph_GPIOB

#define  SPI2_GPIO_CS_PORT			GPIOB
#define  SPI2_GPIO_CS_PIN			GPIO_Pin_7
#define  SPI2_GPIO_CS_CLK			RCC_APB2Periph_GPIOB

#define SPI_2						SPI2
#define SPI2_CLK					RCC_APB1Periph_SPI2
#define	SPI2_RCC_APBxClockCmd		RCC_APB1PeriphClockCmd

#define SPI2_CS_H		GPIO_SetBits(SPI2_GPIO_CS_PORT,SPI2_GPIO_CS_PIN);
#define SPI2_CS_L		GPIO_ResetBits(SPI2_GPIO_CS_PORT,SPI2_GPIO_CS_PIN);			

#endif


#define DEBUG 1
#define FLASH_INFO(fmt,arg...)  printf("<<FLASH-INFO>> "fmt"\n",##arg);
#define FLASH_ERROR(fmt,arg...) printf("<<FLASH-ERROR>> "fmt"\n",##arg);
#define FLASH_DEBUG(fmt,arg...) do{\
                                 if(DEBUG)\
                                 printf("<<EEPROM-DEBUG>> FILE[%s] LINE[%d] DATE[%s]\n"fmt"",__FILE__,__LINE__,__DATE__ ##arg);\
                                 }while(0) //__FILE__ (两个下划线！！！) 调用如下：FLASH_DEBUG();

#define TimeOut	1000	
								 
									 
void SPI1_Init(void);
void SPI2_Init(void);	
u8 SPI1_SendByte(u8 data);
u8 SPI2_SendByte(u8 data);								 
#endif
