#ifndef __ACTION_H__
#define __ACTION_H__


#include "main.h"
#define Pause_Key		    (GPIOA->IDR & GPIO_Pin_0)
void action(void);
u8 UART6_Init(int BaudRate);
void POSITION(int mode,int start,int end);


#endif
