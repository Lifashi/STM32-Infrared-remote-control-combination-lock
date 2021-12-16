#include "bsp_led.h"

void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(LED1_GPIO_CLK|LED2_GPIO_CLK|BEEP_GPIO_CLK,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = LED1_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED1_GPIO_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = LED2_GPIO_PIN;
	GPIO_Init(LED2_GPIO_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(BEEP_GPIO_PORT,&GPIO_InitStruct);
	
	GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);
	GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);
	GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);
}

