#include "remote.h"

// 红外遥控初始化
// 设置IO以及定时器2的输入捕获
void Remote_Init(void)    			  
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;  
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB,ENABLE); //使能PORTB时钟 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	//TIM2 时钟使能 
	
//  寄存器操作关闭 JTAG 使能SW
//	AFIO->MAPR &=~ (7<<24);
//	AFIO->MAPR |=  (2<<24);//关闭JTAG  使能SW
	//部分映像(CH1/ETR/PA15， CH2/PB3， CH3/PA2， CH4/PA3)
//	AFIO->MAPR &=~ (3<<8);
//	AFIO->MAPR |= (1<<8);

//  调用库函数操作关闭 JTAG 使能SW
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE); 	   // 只能部分重映射，完成重映射不能用
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				   // PB3 输入 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	   // 上拉输入 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_3);						   		// 初始化GPIOB3
						  
 	TIM_TimeBaseStructure.TIM_Period = (10000-1);  			 	// 设定计数器自动重装值 最大10ms溢出  
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); 			 	// 预分频器,1M的计数频率,1us加1.	   	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	// 设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // TIM向上计数模式

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 			// 根据指定的参数初始化TIMx

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;  			// 选择输入端 IC2映射到TI2上
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	// 上升沿捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 	// 配置输入分频,不分频 
    TIM_ICInitStructure.TIM_ICFilter = 0X3;						// IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
    TIM_ICInit(TIM2, &TIM_ICInitStructure);						// 初始化定时器输入捕获通道

	TIM_Cmd(TIM2,ENABLE ); 									    // 使能定时器2
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             // TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   // 先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          // 从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             // IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  							// 根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器	

	TIM_ITConfig( TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);		// 允许更新中断 ,允许CC2IE捕获中断
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update|TIM_IT_CC2);
	
}

// 遥控器接收状态
// [7]:收到了引导码标志
// [6]:得到了一个按键的所有信息
// [5]:保留	
// [4]:标记上升沿是否已经被捕获								   
// [3:0]:溢出计时器
u8 	RmtSta=0;	  	  
u32 RmtRec=0;	//红外接收到的数据	   		    
u8  RmtCnt=0;	//按键按下的次数	  

// 处理红外键盘
// 返回值:
// 0,没有任何按键按下
// 其他,按下的按键键值.
u8 Remote_Scan(void)
{        
	u8 sta=0;       
    u8 t1,t2;  
	if(RmtSta&(1<<6))//得到一个按键的所有信息了
	{ 
	    t1=RmtRec>>24;			// 得到地址码
	    t2=(RmtRec>>16)&0xff;	// 得到地址反码 
 	    if((t1==(u8)~t2)&&t1==REMOTE_ID)// 检验遥控识别码(ID)及地址 
	    { 
	        t1=RmtRec>>8;
	        t2=RmtRec; 	
	        if(t1==(u8)~t2)
			{	
				sta = t1;// 键值正确
				RmtSta = 0;
				RmtRec = 0;
			}
		}   
		if((sta==0)||((RmtSta&0X80)==0))// 按键数据错误/遥控已经没有按下了
		{
		 	RmtSta&=~(1<<6);// 清除接收到有效按键标识
			RmtCnt=0;		// 清除按键次数计数器
		}
	}  
    return sta;
}

