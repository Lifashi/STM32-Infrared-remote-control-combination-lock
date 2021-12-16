#ifndef _RED_H
#define _RED_H 
#include "stm32f10x.h" 
#include "io_bit.h" 

#define RDATA 	PBin(3)	 	// �������������

// ����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
// ѡ�õ�ң����ʶ����Ϊ0
#define REMOTE_ID 0      		   

extern u8 	RmtCnt;			// �������µĴ���
extern u8 	RmtSta;	  	  
extern u32  RmtRec;			// ������յ�������	   		    
extern u8   RmtCnt;			// �������µĴ���

void Remote_Init(void);    	// ���⴫��������ͷ���ų�ʼ��
u8 Remote_Scan(void);	 

#endif
