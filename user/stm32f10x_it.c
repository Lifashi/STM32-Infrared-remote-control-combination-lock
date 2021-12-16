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


u16 Dval;		//下降沿时计数器的值
//定时器2中断服务程序	 
void TIM2_IRQHandler(void)
{ 		    	 
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		if(RmtSta&0x80)								// 上次有数据被接收到了
		{	
			RmtSta&=~0X10;							// 取消上升沿已经被捕获标记
			if((RmtSta&0X0F)==0X00)RmtSta|=1<<6;	// 标记已经完成一次按键的键值信息采集
			if((RmtSta&0X0F)<14)RmtSta++;
			else
			{
				RmtSta&=~(1<<7);					// 清空引导标识
				RmtSta&=0XF0;						// 清空计数器	
			}								 	   	
		}							    
	}
	if(TIM_GetITStatus(TIM2,TIM_IT_CC2)!=RESET)
	{	  
		if(RDATA)// 上升沿捕获
		{
  			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);		// CC4P=1	设置为下降沿捕获
			TIM_SetCounter(TIM2,0);									// 清空定时器值
			RmtSta|=0X10;											// 标记上升沿已经被捕获
		}
		else   // 下降沿捕获
		{
			Dval=TIM_GetCapture2(TIM2);								// 读取CCR2也可以清CC2IF标志位
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising);		// CC2P=0	设置为上升沿捕获
			if(RmtSta&0X10)											// 完成一次高电平捕获 
			{
 				if(RmtSta&0X80)						// 接收到了引导码
				{
					
					if(Dval>300&&Dval<800)			// 560为标准值,560us
					{
						RmtRec<<=1;					// 左移一位.
						RmtRec|=0;					// 接收到0	   
					}
					else if(Dval>1400&&Dval<1800)	// 1680为标准值,1680us
					{
						RmtRec<<=1;					// 左移一位.
						RmtRec|=1;					// 接收到1
					}
					else if(Dval>2200&&Dval<2600)	// 得到按键键值增加的信息 2500为标准值2.5ms
					{
						RmtCnt++; 					// 按键次数增加1次
						RmtSta&=0XF0;				// 清空计时器		
					}
 				}
				else if(Dval>4200&&Dval<4700)		// 4500为标准值4.5ms
				{
					RmtSta|=1<<7;					// 标记成功接收到了引导码
					RmtCnt=0;						// 清除按键次数计数器
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
