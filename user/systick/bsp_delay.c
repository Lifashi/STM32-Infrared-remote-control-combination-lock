#include "bsp_delay.h"

#if 0
 __STATIC_INLINE uint32_t SysTick_Config(uint32_t ticks)
 {
 // 不可能的重装载值，超出范围
	 if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk) 
	 {
	 return (1UL);
	 }
 // 设置重装载寄存器
	SysTick->LOAD = (uint32_t)(ticks - 1UL);
 
 // 设置中断优先级
	NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
 // 设置当前数值寄存器
	SysTick->VAL = 0UL;
 // 设置系统定时器的时钟源为 AHBCLK=72M
 // 使能系统定时器中断
 // 使能定时器
	 SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
	 SysTick_CTRL_TICKINT_Msk |
	 SysTick_CTRL_ENABLE_Msk;
	 return (0UL);
 }
 #endif

void Delay_Us(u32 us)
{
	u32 i;
	//SysTick_Config(72); 
	//72000000/1000000
	SysTick_Config(SystemCoreClock/1000000); 
	for(i=0;i<us;i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1	
		while(!(SysTick->CTRL & (1<<16)));
	}
	// 关闭SysTick定时器
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void Delay_Ms(u32 ms)
{
	u32 i;
	//SysTick_Config(72000); 
	SysTick_Config(SystemCoreClock/1000); 
	for(i=0;i<ms;i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1	
		while(!(SysTick->CTRL & (1<<16)));
	}
	// 关闭SysTick定时器
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
