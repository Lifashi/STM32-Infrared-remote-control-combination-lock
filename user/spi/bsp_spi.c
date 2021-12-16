#include "bsp_spi.h"

void SPI_GPIO_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	//开GPIO时钟 SPI时钟
	
	SPI1_GPIO_APBxClockCmd(SPI1_GPIO_SCK_CLK|SPI1_GPIO_MISO_CLK|
						 SPI1_GPIO_MOSI_CLK|SPI1_GPIO_CS_CLK,ENABLE);
	SPI1_RCC_APBxClockCmd(SPI1_CLK,ENABLE);
	
	SPI2_GPIO_APBxClockCmd(SPI2_GPIO_SCK_CLK|SPI2_GPIO_MISO_CLK|
						 SPI2_GPIO_MOSI_CLK|SPI2_GPIO_CS_CLK,ENABLE);
	SPI2_RCC_APBxClockCmd(SPI2_CLK,ENABLE);
	
	//SPI1
	GPIO_InitStruct.GPIO_Pin = SPI1_GPIO_SCK_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_GPIO_SCK_PORT,&GPIO_InitStruct);
	//MOSI引脚 选择复用推挽输出
	GPIO_InitStruct.GPIO_Pin = SPI1_GPIO_MOSI_PIN;
	GPIO_Init(SPI1_GPIO_MOSI_PORT,&GPIO_InitStruct);
	//MISO引脚 选择浮空输入
	GPIO_InitStruct.GPIO_Pin = SPI1_GPIO_MISO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI1_GPIO_MISO_PORT,&GPIO_InitStruct);
	//cs片选引脚 选择推挽输出
	GPIO_InitStruct.GPIO_Pin = SPI1_GPIO_CS_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI1_GPIO_CS_PORT,&GPIO_InitStruct);
	
	//SPI2
	//时钟引脚	选择复用推挽输出
	GPIO_InitStruct.GPIO_Pin = SPI2_GPIO_SCK_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI2_GPIO_SCK_PORT,&GPIO_InitStruct);
	//MOSI引脚 选择复用推挽输出
	GPIO_InitStruct.GPIO_Pin = SPI2_GPIO_MOSI_PIN;
	GPIO_Init(SPI2_GPIO_MOSI_PORT,&GPIO_InitStruct);
	//MISO引脚 选择浮空输入
	GPIO_InitStruct.GPIO_Pin = SPI2_GPIO_MISO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI2_GPIO_MISO_PORT,&GPIO_InitStruct);
	//cs片选引脚 选择推挽输出
	GPIO_InitStruct.GPIO_Pin = SPI2_GPIO_CS_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI2_GPIO_CS_PORT,&GPIO_InitStruct);
}


void SPI_MODE_Config(void)
{
	SPI_InitTypeDef	SPI_InitStruct;

	//双线全双工
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	//主机模式
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	//2分频
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	//FLASH支持0 0、1 1模式 CPHA=1 CPOL=1
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	//校验位 不需要随便填
	SPI_InitStruct.SPI_CRCPolynomial = 0;
	//8位传输模式
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	//设置高位先行
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	//选择软件触发片选引脚
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(SPI_1,&SPI_InitStruct);
	SPI_Init(SPI_2,&SPI_InitStruct);
	SPI_Cmd(SPI_1,ENABLE);
	SPI_Cmd(SPI_2,ENABLE);
}

//初始化SPI
void SPI1_Init(void)
{
	SPI_GPIO_Config();
	SPI_MODE_Config();
}

//初始化SPI
void SPI2_Init(void)
{
	SPI_GPIO_Config();
	SPI_MODE_Config();
}

//SPI1写一个字节数据，并返回一个字节数据
u8 SPI1_SendByte(u8 data)
{
	while(!SPI_I2S_GetFlagStatus(SPI_1,SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI_1,data);
	
	while(!SPI_I2S_GetFlagStatus(SPI_1,SPI_I2S_FLAG_RXNE));
	return SPI_I2S_ReceiveData(SPI_1);
}

//SPI2写一个字节数据，并返回一个字节数据
u8 SPI2_SendByte(u8 data)
{
	while(!SPI_I2S_GetFlagStatus(SPI_2,SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI_2,data);
	
	while(!SPI_I2S_GetFlagStatus(SPI_2,SPI_I2S_FLAG_RXNE));
	return SPI_I2S_ReceiveData(SPI_2);
}


