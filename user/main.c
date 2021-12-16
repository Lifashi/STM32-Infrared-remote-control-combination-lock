#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_delay.h"
#include "bsp_usart.h"
#include "remote.h"
#include "bsp_oled.h"
#include "string.h"

/*****
��Ŀ���ƣ�  ����ң��������
���ܣ�      ����ң����LED����������
����Ա���룺555555���޸ĺ���������Ҫ��
���������룺123456
�������ܣ�
			0~9   ��������
			Left  ɾ�������һ����  ���������
			Right ������������      �����Ҽ���
			UP    �޸�����
			Down  �˳�
			OK 	  ȷ��
			* 	  ��LED1
			# 	  ��LED2
���ң������һ�������޸�Key_Scan������Ӧ��ֵ
*****/

void Key_Scan(u8 key); // ������ֵ
u8   Key[][11]={"0123456789","OK","Left","Right","Up","Down","*","#"};
u8   Key_Num = 80;

/*
// ��ȡң����������ֵ
int main(void)
{
	u8 key;
	USART_GPIO_Config();
	Remote_Init();	
	
	printf("����ʵ���ʼ�����\r\n");
	while(1)
	{
		key=Remote_Scan();	// ����û�а��·��� 0
		if(key)
		{	 
			printf("����ֵ��%d\r\n",key);
		}
	}
}

*/


int main(void)
{
	u8 x = 0,y = 0;
	u8 LED1_Flag = 0,LED2_Flag = 0;
	u8 Password_flag;
	u8 Buff[20],i = 0;
	u8 Password[20];
	u8 PasswordRoot[20]="555555"; // ����Ա����
	u8 Mod = 0;
	u8 Mod_Flag = 0;
	
	LED_GPIO_Config();
	USART_GPIO_Config();
	FLASH_Init();
	OLED_Init();
	Remote_Init();	
	
	SPI_FLASH_ReadData(50100,&Password_flag,1);
	if(Password_flag != 0x88) // ���flash����û�����ݣ���д��һ����ʼ����
	{
		SPI_FLASH_Erase(50000);
		Password_flag = 0x88;
		SPI_FLASH_WriteData(50100 ,&Password_flag,1);
		SPI_FLASH_WriteData(50000 ,(u8*)"123456",sizeof("123456"));
	}
	
	SPI_FLASH_ReadData(50000 ,Password,sizeof(Password)); // ��ȡflash���������
	printf("����ң����������ʼ�����\r\n");
	
	while(1)
	{
		Key_Scan(Remote_Scan());
		switch(Key_Num/10)
		{
			case 0:	if(!Mod || Mod_Flag)  // ����ң���������0~9
					{
						if((!x && !y)&& !Mod_Flag) 	       x += 72; 
						else if((!x && !y)&& Mod_Flag)	   x += 112; 
						Buff[i++] = Key[0][Key_Num];
						OLED_WriteChar(x,y,(u8)'*');
					}
					else
					{
						if(!x && !y)	x += 104;
						Password[i++] = Key[0][Key_Num];
						OLED_WriteChar(x,y,Key[0][Key_Num]);
					}
					x += 8;	
					if(x > 120)
					{
						y = 2;
						x = 0;
					}								
			break;	
					
			case 1: Buff[i] = '\0';	   // OK ȷ��
					if(!Mod || Mod_Flag) 
					{	
						SPI_FLASH_ReadData(50000 ,Password,sizeof(Password));
						if(!strcmp((char*)Buff,(char*)Password))
						{
							OLED_WriteString(50,4,(u8*)"OK");
							printf("������ȷ\r\n");
							Delay_Ms(1000);
							i=0; x=0; y=0;
							Mod_Flag = 0;
							OLED_Clear();
						}
						else if(!strcmp((char*)Buff,(char*)PasswordRoot))
						{
							OLED_WriteString(50,4,(u8*)"OK");
							printf("������ȷ\r\n");
							Delay_Ms(1000);
							i=0; x=0; y=0;
							Mod_Flag = 0;
							OLED_Clear();
						}
						else
						{
							OLED_WriteString(50,4,(u8*)"ERROR");
							printf("�������\r\n");
							Delay_Ms(1000);
							i=0; x=0; y=0; 
							OLED_Clear();
						}
						
					}
					else								
					{
						OLED_Clear();
						SPI_FLASH_Erase(50000);
						Password_flag = 0x88;
						SPI_FLASH_WriteData(50100 ,&Password_flag,1);
						SPI_FLASH_WriteData(50000 ,(u8*)Password,sizeof(Password));
						OLED_WriteString(0,3,(u8*)"Password_Mod OK");
						printf("�����޸ĳɹ�:%s\r\n",Password);
						Delay_Ms(1000);
						i=0; x=0; y=0; Mod=0;
						OLED_Clear();
					}								
					memset((char*)Password,0,20);	
			break;

			case 2:if(y && !x)		    // Left ɾ��һ��
				   {
					 y -= 2;
					 x = 128;
				   }
					x -= 8;
					i--;
					OLED_WriteChar(x,y,(u8)' ');	
			break;
				   
			case 3: i=0; x=0; y=0;		// Right ��������
				    OLED_Clear();					
		    break;
				   
			case 4: OLED_Clear();       //UP �޸�����
					Mod = 1;
				    Mod_Flag = 1;
				    i=0;x=0;y=0;					
		    break;
				   
			case 5: OLED_Clear();		 //Down  �˳�
					Mod = 0;
				    Mod_Flag = 0;
				    i=0;x=0;y=0;					
		    break;
				   
			case 6: LED1_TOGGLE  LED1_Flag = !LED1_Flag;  // * ��LED1
					if(LED1_Flag) OLED_WriteString(20,5,(u8*)"LED1_NO");
					else	      OLED_WriteString(20,5,(u8*)"LED1_OFF");
					BEEP_NO; Delay_Ms(300); BEEP_OFF;  
					Delay_Ms(1000);
					OLED_Clear();					
			break;
				
			case 7: LED2_TOGGLE; LED2_Flag = !LED2_Flag;	// # ��LED2
					if(LED2_Flag) OLED_WriteString(20,5,(u8*)"LED2_NO");
					else	      OLED_WriteString(20,5,(u8*)"LED2_OFF");
					BEEP_NO; Delay_Ms(300); BEEP_OFF;  
					Delay_Ms(1000);
					OLED_Clear();					
			break;
					
			case 8: if(!Mod_Flag && !Mod)				// ���
						OLED_WriteString(0,0,(u8*)"Password:");
					else if(!Mod_Flag && Mod)
						OLED_WriteString(0,0,(u8*)"Password Mod:");
					else 
						OLED_WriteString(0,0,(u8*)"Password Root:");
													
		}
		Key_Num = 80; // Ĭ�Ͻ��� case8
	}
}

void Key_Scan(u8 key)
{	 
	switch(key)
	{		
		case 104:Key_Num = 0;		break;	
		case 48: Key_Num = 1;		break;	    
		case 24: Key_Num = 2;		break;
		case 122:Key_Num = 3;		break;	 
		case 16: Key_Num = 4;		break;		  
		case 56: Key_Num = 5;		break;		  
		case 90: Key_Num = 6;		break;	   
		case 66: Key_Num = 7;		break;	  
		case 74: Key_Num = 8;		break;		   
		case 82: Key_Num = 9;		break;		    	  	
		case 168:Key_Num = 10;		break;	// OK
		case 224:Key_Num = 20;		break;  // Left
		case 144:Key_Num = 30;		break;	// Right
		case 2:  Key_Num = 40;		break;	// Up
		case 152:Key_Num = 50;		break;	// Down
		case 34: Key_Num = 60;		break;	// *
		case 194:Key_Num = 70;		break;	// #
	}	
}



