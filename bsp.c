#include "bsp.h"

void BSP_Init(void)
{
	SysTick_Configuration();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
  LED_KEY_Init();
	power_Init();
	Init_CAN1();
	SV_Init();
	TIM3_Int_Init(50-1,8400-1);  //84M  84M/psc = 10Khz  10Khz * 50 = 5ms
//		TIM9_Int_Init(5000-1,168-1);
//		TIM2_CH3_Cap_Init(0XFFFFFFFF,84-1);
	UART1_Init(100000);
	TIM5_PWM_Init(2000-1,840-1);
	TIM12_PWM_Init(25000-1,840-1);
	UART2_Init(115200);
	UART6_Init(115200);
	Pause_Key_Init();
}

