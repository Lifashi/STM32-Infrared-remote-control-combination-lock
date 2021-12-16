#include "bsp_usart.h"

struct USARTX Usart_x;

#if 1
void USART_NVIC_Config(void)
{
	NVIC_InitTypeDef	NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel = USARTx_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
}
#endif
void USART_GPIO_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	USART_InitTypeDef	USART_InitStruct;
	
	RCC_APB2PeriphClockCmd(USART_GPIO_RX_CLK|USART_GPIO_TX_CLK|USART_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = USART_GPIO_TX_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_GPIO_TX_PORT,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = USART_GPIO_RX_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_GPIO_RX_PORT,&GPIO_InitStruct);
	
	USART_InitStruct.USART_BaudRate = USART_BaudRatex;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USARTx,&USART_InitStruct);
	
	USART_NVIC_Config();
	USART_ITConfig(USARTx,USART_IT_RXNE,ENABLE);
	USART_Cmd(USARTx,ENABLE);
}

void SendByte(USART_TypeDef* pUSARTx, u8 data)
{
	USART_SendData(pUSARTx,data);
	while(!USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE));
}

void Send_TwoByte(USART_TypeDef* pUSARTx, u16 data)
{
	u8 temp_h,temp_l;
	
	temp_l = data & 0X00FF;
	temp_h = (data & 0XFF00) >> 8; 
	SendByte(pUSARTx,temp_h);
	SendByte(pUSARTx,temp_l);
}

void Send_Array(USART_TypeDef* pUSARTx, u8 *array , u8 num)
{
	u8 i;
	for(i=0;i<num;i++)
	{
		SendByte(pUSARTx,*(array+i));
	}
	while(!USART_GetFlagStatus(pUSARTx,USART_FLAG_TC));
}

void Send_Str(USART_TypeDef* pUSARTx, u8 *str)
{
	while(*str)
	{
		SendByte(pUSARTx,*(str++));
	}
	while(!USART_GetFlagStatus(pUSARTx,USART_FLAG_TC));
}

void Read_Str1(void)
{
	Usart_x.Len=0;
	Usart_x.USART_RX_FLAG=0;
	char ch ;
	while(1)
	{
		ch = getchar();
		if(ch != '\r'&& ch != '\n')
		{
			Usart_x.USART_RX_BUFF[Usart_x.Len++]=ch;
		}
		if(ch == '\n')
		{
			Usart_x.USART_RX_BUFF[Usart_x.Len]='\0';
			Usart_x.USART_RX_FLAG=1;
			break;
		}
		
	}
	
}
void Read_Str(char *str)
{
	char ch ,*p =str;
	while(1)
	{
		ch = getchar();
		if(ch == '\r'||ch == '\n')
			break;
		*(p++) = ch;
	}
	*p='\0';
	while(getchar() != '\n');
}

int fputc(int data ,FILE *fp)
{
	SendByte(USARTx, data);
	while(!USART_GetFlagStatus(USARTx,USART_FLAG_TXE));
	return 0;
}

int fgetc(FILE *fp)
{
	while(!USART_GetFlagStatus(USARTx,USART_FLAG_RXNE));
	return (int)USART_ReceiveData(USARTx);
}


