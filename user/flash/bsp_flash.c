#include "bsp_flash.h"

void FLASH_Init(void)
{
	SPI1_Init();
}
//дһ���ֽ����ݣ�������һ���ֽ�����
u8 SPI_FLASH_SendByte(u8 data)
{
	return SPI1_SendByte(data);
}
//��ȡflash ID

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
//FLASH����͹���ģʽ
void  FLASH_Power_Downr_Enable(void)
{
	SPI1_CS_L;
	SPI_FLASH_SendByte(Power_Down);
	SPI1_CS_H;
}
//FLASH�˳��͹���ģʽ���������豸ID
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
//дʹ�ܣ�ÿ��д�붼Ҫ��ʹ��
void SPI_FLASH_Write_ENABLE(void)
{
	SPI1_CS_L;
	SPI_FLASH_SendByte(Write_Enable);
	SPI1_CS_H;
}

//��flash BUSYλ �ж��Ƿ�æµ(1)
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

//����flash 4kb�ռ䣨��������1)
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

//��flash��ȡ����
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

//��flashд��һ������
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
//��flash��һ������
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


//��flashд������(ÿ�����ֻ��д��256byte)
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
		FLASH_ERROR("ÿ��д�����256�ֽڣ�����");
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

//д��һҳ(ÿ�����ֻ��д��256byte)
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
		FLASH_ERROR("ÿ��д�����256�ֽڣ�����");
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

//��ȡ��ҳ����
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

//����д���ҳ����
void SPI_FLASH_Write_BufferData(u32 addr ,u8 *data,u32 WriteNumByte)
{
	u8 TempByte = 0,AddrFlag = 0,NumPage = 0,NumByte = 0;
	
	//addr=5 WriteNumByte=255  
	AddrFlag = addr % FLASH_PageByte;   //�����ַ�Ƿ����
	NumByte = FLASH_PageByte - addr; //������ַ�����ж��ٸ��ֽ�
	NumPage = WriteNumByte / FLASH_PageByte;  //����ж���ҳ
	TempByte = WriteNumByte % FLASH_PageByte; //�����ҳд��ʣ����ֽ�
	//�Ƿ����
	if(!AddrFlag)
	{
		//���룬�Ȱ�ҳд��
		while(NumPage--)
		{
			SPI_FLASH_WritePageData(addr,data,FLASH_PageByte);
			data += FLASH_PageByte;
			addr += FLASH_PageByte;
		}
		//�ٰ�ʣ����ֽ�д��
		SPI_FLASH_WritePageData(addr,data,TempByte);
	}
	//������
	else
	{
		//�ж��Ƿ�һҳ
		if(!NumPage)
		{
			//�ж�һҳʣ���ֽڿռ��Ƿ����д���ֽ���
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
			//��д�����ַ���뻹ʣ�Ŀռ�
			SPI_FLASH_WritePageData(addr,data,NumByte);
			
			WriteNumByte -=NumByte;
			NumPage = WriteNumByte / FLASH_PageByte;
			TempByte = WriteNumByte % FLASH_PageByte;
			
			addr += NumByte;
			data += NumByte;
			//�ж��Ƿ�һҳ
			if(!NumPage)
			{
				SPI_FLASH_WritePageData(addr,data,WriteNumByte);
			}
			else
			{
				//�Ȱ�ҳд��
				while(NumPage--)
				{
					SPI_FLASH_WritePageData(addr,data,FLASH_PageByte);
					data += FLASH_PageByte;
					addr += FLASH_PageByte;
				}
				//�ٰ�ʣ����ֽ�д��
				SPI_FLASH_WritePageData(addr,data,TempByte);
			}
	   }
   }
}
