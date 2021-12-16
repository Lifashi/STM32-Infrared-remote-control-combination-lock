#ifndef _BSP_USART_H
#define _BSP_USART_H

#include "stm32f10x.h"
#include <stdio.h>
#include "string.h"

struct USARTX
{
	u8 USART_RX_BUFF[10];
	u8 USART_RX_IT_BUFF[10];
	u8 Len;
	u8 USART_RX_FLAG;
	u8 USART_RX_IT_FLAG;
	u8 USART_RX_Time_FLAG;
};
extern struct USARTX Usart_x;

#define USART_GPIO_RX_PORT   GPIOA 
#define USART_GPIO_RX_Pin	 GPIO_Pin_10
#define USART_GPIO_RX_CLK	 RCC_APB2Periph_GPIOA

#define USART_GPIO_TX_PORT   GPIOA 
#define USART_GPIO_TX_Pin	 GPIO_Pin_9
#define USART_GPIO_TX_CLK	 RCC_APB2Periph_GPIOA

#define USARTx				 USART1
#define USART_CLK			 RCC_APB2Periph_USART1
//#define USART_BaudRatex		 9600
#define USART_BaudRatex		 115200
#define USARTx_IRQn			 USART1_IRQn

void USART_GPIO_Config(void);
void SendByte(USART_TypeDef* pUSARTx, u8 data);
void Send_TwoByte(USART_TypeDef* pUSARTx, u16 data);
void Send_Array(USART_TypeDef* pUSARTx, u8 *array , u8 num);
void Send_Str(USART_TypeDef* pUSARTx, u8 *str);
void Read_Str(char *str);
void Read_Str1(void);
int fputc(int data ,FILE *fp);
int fgetc(FILE *fp);
#endif
