#ifndef _RED_H
#define _RED_H 
#include "stm32f10x.h" 
#include "io_bit.h" 

#define RDATA 	PBin(3)	 	// 红外数据输入脚

// 红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
// 选用的遥控器识别码为0
#define REMOTE_ID 0      		   

extern u8 	RmtCnt;			// 按键按下的次数
extern u8 	RmtSta;	  	  
extern u32  RmtRec;			// 红外接收到的数据	   		    
extern u8   RmtCnt;			// 按键按下的次数

void Remote_Init(void);    	// 红外传感器接收头引脚初始化
u8 Remote_Scan(void);	 

#endif
