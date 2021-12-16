#include "bsp_delay.h"

#if 0
 __STATIC_INLINE uint32_t SysTick_Config(uint32_t ticks)
 {
 // �����ܵ���װ��ֵ��������Χ
	 if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk) 
	 {
	 return (1UL);
	 }
 // ������װ�ؼĴ���
	SysTick->LOAD = (uint32_t)(ticks - 1UL);
 
 // �����ж����ȼ�
	NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
 // ���õ�ǰ��ֵ�Ĵ���
	SysTick->VAL = 0UL;
 // ����ϵͳ��ʱ����ʱ��ԴΪ AHBCLK=72M
 // ʹ��ϵͳ��ʱ���ж�
 // ʹ�ܶ�ʱ��
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
		// ����������ֵ��С��0��ʱ��CRTL�Ĵ�����λ16����1	
		while(!(SysTick->CTRL & (1<<16)));
	}
	// �ر�SysTick��ʱ��
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void Delay_Ms(u32 ms)
{
	u32 i;
	//SysTick_Config(72000); 
	SysTick_Config(SystemCoreClock/1000); 
	for(i=0;i<ms;i++)
	{
		// ����������ֵ��С��0��ʱ��CRTL�Ĵ�����λ16����1	
		while(!(SysTick->CTRL & (1<<16)));
	}
	// �ر�SysTick��ʱ��
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
