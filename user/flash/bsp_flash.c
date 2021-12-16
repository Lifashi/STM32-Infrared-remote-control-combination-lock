#include "bsp_flash.h"

void FLASH_Init(void)
{
	SPI1_Init();
}
//写一个字节数据，并返回一个字节数据
u8 SPI_FLASH_SendByte(u8 data)
{
	return SPI1_SendByte(data);
}
//读取flash ID

u32 SPI_FLASH_ReadID(void)
{
	u32 FLASH_ID;
	SPI1_CS_L;
	SPI_FLASH_SendByte(W25Q16_ID);
	FLASH_ID = SPI_FLASH_SendByte(DEMMY);
	FLASH_ID <<= 8; 
	FLASH_ID |= SPI_FLASH_SendByte(DEMMY);
	FLASH_ID <<= 8;
	FLASH_ID |= SPI_FLASH_SendByte(DEMMY);
	SPI1_CS_H;
	return FLASH_ID;
}
//FLASH进入低功耗模式
void  FLASH_Power_Downr_Enable(void)
{
	SPI1_CS_L;
	SPI_FLASH_SendByte(Power_Down);
	SPI1_CS_H;
}
//FLASH退出低功耗模式，并返回设备ID
u8  FLASH_Power_Downr_Disable(void)
{
	u8 Device_ID;
	SPI1_CS_L;
	SPI_FLASH_SendByte(Release_Power_Down);
	SPI_FLASH_SendByte(DEMMY);
	SPI_FLASH_SendByte(DEMMY);
	SPI_FLASH_SendByte(DEMMY);
	Device_ID = SPI_FLASH_SendByte(DEMMY);
	SPI1_CS_H;
	return Device_ID;
}
//写使能，每次写入都要先使能
void SPI_FLASH_Write_ENABLE(void)
{
	SPI1_CS_L;
	SPI_FLASH_SendByte(Write_Enable);
	SPI1_CS_H;
}

//读flash BUSY位 判断是否忙碌(1)
void SPI_FLASH_Read_Status(void)
{
	u8 Status;
	SPI1_CS_L;
	SPI_FLASH_SendByte(Read_Status);
	do
	{
		Status = SPI_FLASH_SendByte(DEMMY);
	}
	while(Status & 0x1); 
	SPI1_CS_H;
}

//擦除flash 4kb空间（擦除后置1)
void SPI_FLASH_Erase(u32 addr)
{
	SPI_FLASH_Write_ENABLE();
	SPI1_CS_L;
	SPI_FLASH_SendByte(Sector_Erase_4);
	SPI_FLASH_SendByte((addr >> 16)&0xff);
	SPI_FLASH_SendByte((addr >> 8)&0xff);
	SPI_FLASH_SendByte(addr&0xff);
	SPI1_CS_H;
	SPI_FLASH_Read_Status();
}

//向flash读取数据
void SPI_FLASH_ReadData(u32 addr ,u8 *data,u32 numbyte)
{
	SPI1_CS_L;
	SPI_FLASH_SendByte(Read_Data);
	SPI_FLASH_SendByte((addr >> 16)&0xff);
	SPI_FLASH_SendByte((addr >> 8)&0xff);
	SPI_FLASH_SendByte(addr&0xff);
	while(numbyte--)
	{
		*data=SPI_FLASH_SendByte(DEMMY);
		data++;
	}
	SPI1_CS_H;
}

//向flash写入一个整数
void SPI_FLASH_WriteNum(u32 addr ,u32 data)
{
	u8 i=0,buffer[20];
	SPI_FLASH_Write_ENABLE();
	SPI1_CS_L;
	SPI_FLASH_SendByte(Write_Data);
	SPI_FLASH_SendByte((addr >> 16)&0xff);
	SPI_FLASH_SendByte((addr >> 8)&0xff);
	SPI_FLASH_SendByte(addr&0xff);
	
	if(data < 10)
		SPI_FLASH_SendByte(0);	
	sprintf((char*)buffer,"%d",data);
	while(buffer[i])
	{
		SPI_FLASH_SendByte(buffer[i++]);
	}
	SPI1_CS_H;
	SPI_FLASH_Read_Status();
}
//向flash读一个整数
void SPI_FLASH_ReadNum(u32 addr,u8 *Num ,u8 NumByte)
{
	u8 i=0,Buffer[20]="\0";
	u16 data;
	SPI1_CS_L;
	SPI_FLASH_SendByte(Read_Data);
	SPI_FLASH_SendByte((addr >> 16)&0xff);
	SPI_FLASH_SendByte((addr >> 8)&0xff);
	SPI_FLASH_SendByte(addr&0xff);
	while(NumByte--)
	{
		Buffer[i++] = SPI_FLASH_SendByte(DEMMY);
	}
	SPI1_CS_H;
	i=0;
	if(Buffer[i]=='\0')
		i=1;
	sscanf((char*)Buffer+i,"%d",(int*)&data);
	*Num=data;
}


//向flash写入数据(每次最多只能写入256byte)
void SPI_FLASH_WriteData(u32 addr ,u8 *data,u32 numbyte)
{
	SPI_FLASH_Write_ENABLE();
	SPI1_CS_L;
	SPI_FLASH_SendByte(Write_Data);
	SPI_FLASH_SendByte((addr >> 16)&0xff);
	SPI_FLASH_SendByte((addr >> 8)&0xff);
	SPI_FLASH_SendByte(addr&0xff);
	if(numbyte>256)
	{
		FLASH_ERROR("每次写入最多256字节！！！");
		return;
	}
	while(numbyte--)
	{
		SPI_FLASH_SendByte(*data);
		data++;
	}
	SPI1_CS_H;
	SPI_FLASH_Read_Status();
}

//写入一页(每次最多只能写入256byte)
void SPI_FLASH_WritePageData(u32 addr ,u8 *data,u32 numbyte)
{
	SPI_FLASH_Write_ENABLE();
	SPI1_CS_L;
	SPI_FLASH_SendByte(Write_Data);
	SPI_FLASH_SendByte((addr >> 16)&0xff);
	SPI_FLASH_SendByte((addr >> 8)&0xff);
	SPI_FLASH_SendByte(addr&0xff);
	if(numbyte>256)
	{
		FLASH_ERROR("每次写入最多256字节！！！");
		return;
	}
	while(numbyte--)
	{
		SPI_FLASH_SendByte(*data);
		data++;
	}
	SPI1_CS_H;
	SPI_FLASH_Read_Status();
}

//读取多页数据
void SPI_FLASH_ReadPageData(u32 addr ,u8 *data,u32 numbyte)
{
	SPI1_CS_L;
	SPI_FLASH_SendByte(Read_Data);
	SPI_FLASH_SendByte((addr >> 16)&0xff);
	SPI_FLASH_SendByte((addr >> 8)&0xff);
	SPI_FLASH_SendByte(addr&0xff);
	while(numbyte--)
	{
		*data=SPI_FLASH_SendByte(DEMMY);
		data++;
	}
	SPI1_CS_H;
}

//连续写入多页数据
void SPI_FLASH_Write_BufferData(u32 addr ,u8 *data,u32 WriteNumByte)
{
	u8 TempByte = 0,AddrFlag = 0,NumPage = 0,NumByte = 0;
	
	//addr=5 WriteNumByte=255  
	AddrFlag = addr % FLASH_PageByte;   //求出地址是否对齐
	NumByte = FLASH_PageByte - addr; //求出离地址对齐有多少个字节
	NumPage = WriteNumByte / FLASH_PageByte;  //求出有多少页
	TempByte = WriteNumByte % FLASH_PageByte; //求出按页写入剩余的字节
	//是否对齐
	if(!AddrFlag)
	{
		//对齐，先按页写入
		while(NumPage--)
		{
			SPI_FLASH_WritePageData(addr,data,FLASH_PageByte);
			data += FLASH_PageByte;
			addr += FLASH_PageByte;
		}
		//再把剩余的字节写入
		SPI_FLASH_WritePageData(addr,data,TempByte);
	}
	//不对齐
	else
	{
		//判断是否够一页
		if(!NumPage)
		{
			//判断一页剩余字节空间是否大于写入字节数
			if(NumByte>WriteNumByte)
			{
				SPI_FLASH_WritePageData(addr,data,TempByte);
			}
			else
			{
				SPI_FLASH_WritePageData(addr,data,NumByte);
				
				TempByte = WriteNumByte - NumByte;
				addr += NumByte;
				data += NumByte;
				SPI_FLASH_WritePageData(addr,data,TempByte);
			}
				
		}
		else
		{
			//先写入离地址对齐还剩的空间
			SPI_FLASH_WritePageData(addr,data,NumByte);
			
			WriteNumByte -=NumByte;
			NumPage = WriteNumByte / FLASH_PageByte;
			TempByte = WriteNumByte % FLASH_PageByte;
			
			addr += NumByte;
			data += NumByte;
			//判断是否够一页
			if(!NumPage)
			{
				SPI_FLASH_WritePageData(addr,data,WriteNumByte);
			}
			else
			{
				//先按页写入
				while(NumPage--)
				{
					SPI_FLASH_WritePageData(addr,data,FLASH_PageByte);
					data += FLASH_PageByte;
					addr += FLASH_PageByte;
				}
				//再把剩余的字节写入
				SPI_FLASH_WritePageData(addr,data,TempByte);
			}
	   }
   }
}
