/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_usart.h"
#include "remote.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

//void EXTI0_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Linex))
//	{
//	}
//	EXTI_ClearITPendingBit(EXTI_Linex);
//}


u16 Dval;		//�½���ʱ��������ֵ
//��ʱ��2�жϷ������	 
void TIM2_IRQHandler(void)
{ 		    	 
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		if(RmtSta&0x80)								// �ϴ������ݱ����յ���
		{	
			RmtSta&=~0X10;							// ȡ���������Ѿ���������
			if((RmtSta&0X0F)==0X00)RmtSta|=1<<6;	// ����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ�
			if((RmtSta&0X0F)<14)RmtSta++;
			else
			{
				RmtSta&=~(1<<7);					// ���������ʶ
				RmtSta&=0XF0;						// ��ռ�����	
			}								 	   	
		}							    
	}
	if(TIM_GetITStatus(TIM2,TIM_IT_CC2)!=RESET)
	{	  
		if(RDATA)// �����ز���
		{
  			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);		// CC4P=1	����Ϊ�½��ز���
			TIM_SetCounter(TIM2,0);									// ��ն�ʱ��ֵ
			RmtSta|=0X10;											// ����������Ѿ�������
		}
		else   // �½��ز���
		{
			Dval=TIM_GetCapture2(TIM2);								// ��ȡCCR2Ҳ������CC2IF��־λ
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising);		// CC2P=0	����Ϊ�����ز���
			if(RmtSta&0X10)											// ���һ�θߵ�ƽ���� 
			{
 				if(RmtSta&0X80)						// ���յ���������
				{
					
					if(Dval>300&&Dval<800)			// 560Ϊ��׼ֵ,560us
					{
						RmtRec<<=1;					// ����һλ.
						RmtRec|=0;					// ���յ�0	   
					}
					else if(Dval>1400&&Dval<1800)	// 1680Ϊ��׼ֵ,1680us
					{
						RmtRec<<=1;					// ����һλ.
						RmtRec|=1;					// ���յ�1
					}
					else if(Dval>2200&&Dval<2600)	// �õ�������ֵ���ӵ���Ϣ 2500Ϊ��׼ֵ2.5ms
					{
						RmtCnt++; 					// ������������1��
						RmtSta&=0XF0;				// ��ռ�ʱ��		
					}
 				}
				else if(Dval>4200&&Dval<4700)		// 4500Ϊ��׼ֵ4.5ms
				{
					RmtSta|=1<<7;					// ��ǳɹ����յ���������
					RmtCnt=0;						// �����������������
				}						 
			}
			RmtSta&=~(1<<4);
		}				 		     	    					   
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update|TIM_IT_CC2);	 	    
}




/*
void USART1_IRQHandler(void)
{
	u8 temp;
	if(USART_GetITStatus(USARTx,USART_IT_RXNE))
	{
		temp=USART_ReceiveData(USARTx);
		USART_SendData(USARTx,temp);
	}
}*/
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
