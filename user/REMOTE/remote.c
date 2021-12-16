#include "remote.h"

// ����ң�س�ʼ��
// ����IO�Լ���ʱ��2�����벶��
void Remote_Init(void)    			  
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;  
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB,ENABLE); //ʹ��PORTBʱ�� 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	//TIM2 ʱ��ʹ�� 
	
//  �Ĵ��������ر� JTAG ʹ��SW
//	AFIO->MAPR &=~ (7<<24);
//	AFIO->MAPR |=  (2<<24);//�ر�JTAG  ʹ��SW
	//����ӳ��(CH1/ETR/PA15�� CH2/PB3�� CH3/PA2�� CH4/PA3)
//	AFIO->MAPR &=~ (3<<8);
//	AFIO->MAPR |= (1<<8);

//  ���ÿ⺯�������ر� JTAG ʹ��SW
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE); 	   // ֻ�ܲ�����ӳ�䣬�����ӳ�䲻����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				   // PB3 ���� 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	   // �������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_3);						   		// ��ʼ��GPIOB3
						  
 	TIM_TimeBaseStructure.TIM_Period = (10000-1);  			 	// �趨�������Զ���װֵ ���10ms���  
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); 			 	// Ԥ��Ƶ��,1M�ļ���Ƶ��,1us��1.	   	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	// ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // TIM���ϼ���ģʽ

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 			// ����ָ���Ĳ�����ʼ��TIMx

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;  			// ѡ������� IC2ӳ�䵽TI2��
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	// �����ز���
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 	// ���������Ƶ,����Ƶ 
    TIM_ICInitStructure.TIM_ICFilter = 0X3;						// IC4F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
    TIM_ICInit(TIM2, &TIM_ICInitStructure);						// ��ʼ����ʱ�����벶��ͨ��

	TIM_Cmd(TIM2,ENABLE ); 									    // ʹ�ܶ�ʱ��2
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             // TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   // ��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          // �����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             // IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  							// ����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���	

	TIM_ITConfig( TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);		// ��������ж� ,����CC2IE�����ж�
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update|TIM_IT_CC2);
	
}

// ң��������״̬
// [7]:�յ����������־
// [6]:�õ���һ��������������Ϣ
// [5]:����	
// [4]:����������Ƿ��Ѿ�������								   
// [3:0]:�����ʱ��
u8 	RmtSta=0;	  	  
u32 RmtRec=0;	//������յ�������	   		    
u8  RmtCnt=0;	//�������µĴ���	  

// ����������
// ����ֵ:
// 0,û���κΰ�������
// ����,���µİ�����ֵ.
u8 Remote_Scan(void)
{        
	u8 sta=0;       
    u8 t1,t2;  
	if(RmtSta&(1<<6))//�õ�һ��������������Ϣ��
	{ 
	    t1=RmtRec>>24;			// �õ���ַ��
	    t2=(RmtRec>>16)&0xff;	// �õ���ַ���� 
 	    if((t1==(u8)~t2)&&t1==REMOTE_ID)// ����ң��ʶ����(ID)����ַ 
	    { 
	        t1=RmtRec>>8;
	        t2=RmtRec; 	
	        if(t1==(u8)~t2)
			{	
				sta = t1;// ��ֵ��ȷ
				RmtSta = 0;
				RmtRec = 0;
			}
		}   
		if((sta==0)||((RmtSta&0X80)==0))// �������ݴ���/ң���Ѿ�û�а�����
		{
		 	RmtSta&=~(1<<6);// ������յ���Ч������ʶ
			RmtCnt=0;		// �����������������
		}
	}  
    return sta;
}

