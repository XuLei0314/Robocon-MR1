#include "timer.h"
#include "base.h"
#include "stm32f4xx.h"
#include "chassis.h"
u8  TIM2CH3_CAPTURE_STA=0,ppm_rx_sta=0,ppm_rx_num=0;	//输入捕获状态		    				
u32	TIM2CH3_CAPTURE_VAL;	//输入捕获值
u16 ppm_rx[10];//ppm_rx[0]   1   接收到ppm数据

s16 current_M3508[4];
extern chassis_t chassis;
extern moto_measure_t moto_chassis[3];
extern pid_t pid_spd[3];

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 
	TIM_TimeBaseInitStructure.TIM_Period = arr; 
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; 
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM3,ENABLE); 
}

void TIM9_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE); 
	TIM_TimeBaseInitStructure.TIM_Period = arr; 
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; 
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE); 
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_BRK_TIM9_IRQn; 
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM9,ENABLE); 
}

TIM_ICInitTypeDef TIM2_ICInitStructure;

void TIM2_CH3_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); //TIM5 ????
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //?? PORTA ??
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //?? 100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //??????
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //??
	GPIO_Init(GPIOB,&GPIO_InitStructure); //??? PA0
	GPIO_SetBits(GPIOB,GPIO_Pin_10);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc; //?????
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
	TIM_TimeBaseStructure.TIM_Period=arr; //??????
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_3; //????? IC1 ??? TI1 ?
	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling; //?????
	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //??? TI1 ?
	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //??????,???
	TIM2_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ??????? ???
	TIM_ICInit(TIM2, &TIM2_ICInitStructure); //??? TIM5 ??????

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//????? 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;//????? 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ????
	NVIC_Init(&NVIC_InitStructure); //?????????? VIC ????
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC3,ENABLE);//?????????
	TIM_Cmd(TIM2,ENABLE ); 
	
}

void TIM2_IRQHandler(void)
{
	
 	if((TIM2CH3_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM2CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM2CH3_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM2CH3_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM2CH3_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM2CH3_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)//捕获3发生捕获事件
		{	
			if(TIM2CH3_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM2CH3_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
				TIM2CH3_CAPTURE_VAL=TIM_GetCapture3(TIM2);
			 // printf("Rising is %d\n",TIM2CH3_CAPTURE_VAL);
		   		TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Falling); //CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM2CH3_CAPTURE_STA=0;			//清空
				TIM2CH3_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM2,0);
				TIM2CH3_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
		   		TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Rising);		//CC1P=1 设置为下降沿捕获
				TIM_Cmd(TIM2,ENABLE);
			}		    
		}			     	    					   
 	}
	
	//处理帧数据
		if(TIM2CH3_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
//		  temp=TIM5CH1_CAPTURE_STA&0X3F;
//			temp*=65536;//溢出时间总和
//			temp+=TIM5CH1_CAPTURE_VAL;//得到总的高电平时间
			if(ppm_rx_sta==1) {ppm_rx[ppm_rx_num+1]=TIM2CH3_CAPTURE_VAL;ppm_rx_num++;}//printf("TIM5CH1_CAPTURE_VAL:%d\r\n",TIM5CH1_CAPTURE_VAL);
			if(4>TIM2CH3_CAPTURE_STA&0X3F>0||TIM2CH3_CAPTURE_VAL>3000) ppm_rx_sta++;//低电平时间大于3000us为起始帧
			if(ppm_rx_sta==2) {ppm_rx_sta=0;ppm_rx[0]=1;ppm_rx_num=0;}//printf("receive\r\n");//ppm_rx_sta   1 表示接收到同步帧/ 2接收到到下一起始帧 ppm数据接收完毕
			
			TIM2CH3_CAPTURE_STA=0;//开启下一次捕获
		}						
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC3|TIM_IT_Update); //清除中断标志位
}
int time;
volatile u16 temp = 0;
extern char bt_flag;
int flag2=0;
void TIM3_IRQHandler(void)
{
	
	static u16 led_cnt;
	static int j;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) 
	{
		time++;
		if(time>100)
		{
			flag2++;
			time=0;
		}
//		count1++;
//		LED0=!LED0;
//		temp++; 
//		if(time>4000) 
//			temp = 0;
		
		if(temp % 10 == 0) //50ms  2Hz
		{
//			if(led_cnt++ >27)
//				led_cnt = 0;
//			if(led_cnt<2) 
//				GREEN_ON;
//			else if(led_cnt<5) 
//				GREEN_OFF;
//			else if(led_cnt<7)
//				GREEN_ON;
//			else  
//				GREEN_OFF;
			}
		  mecanum_calc(chassis.vx, chassis.vy, chassis.vw, MAX_WHEEL_SPEED,chassis.wheel_w_speed);
	    for(j=0;j<4;j++)
			current_M3508[j] = pid_calc(&pid_spd[j],moto_chassis[j].speed_rpm * PI/30,chassis.wheel_w_speed[j] * 5);
//			if(bt_flag)
				Set_Motor_Speed_Can(current_M3508[0],current_M3508[1],current_M3508[2],current_M3508[3]);
			}
			TIM_ClearITPendingBit(TIM3,TIM_IT_Update); 
}
volatile u16 temp9 = 0;
void TIM1_BRK_TIM9_IRQHandler(void)
{
//		static u16 led_cnt;
	 if (TIM_GetITStatus(TIM9, TIM_IT_Update) != RESET) 
		 { 
//			 
//			 			temp9++; if(temp9>50000) temp9 = 0;
//		
//		
//			if(temp9 % 10 == 0) //50ms  2Hz
//			{
//				if(led_cnt++ >27)led_cnt = 0;
//				if(led_cnt<2)  RED_ON;
//				else if(led_cnt<5) RED_OFF;
//				else if(led_cnt<7) RED_ON;
//				else  RED_OFF;

//			}
			 TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
		 }
}

//TIM14 PWM 部分初始化
//PWM 输出初始化
//arr：自动重装值 psc：时钟预分频数
void TIM5_PWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);//TIM14 时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE); //使能 PORTF 时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE); //使能 PORTF 时钟
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource10,GPIO_AF_TIM5); //GF9 复用为 TIM14
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource11,GPIO_AF_TIM5); //GF9 复用为 TIM14
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource12,GPIO_AF_TIM5); //GF9 复用为 TIM14
	GPIO_PinAFConfig(GPIOI,GPIO_PinSource0,GPIO_AF_TIM5); //GF9 复用为 TIM14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12; //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度 50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOH,&GPIO_InitStructure); //初始化 PF9
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度 50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOI,&GPIO_InitStructure); //初始化 PF9
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc; //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr; //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//STM32F4 开发指南(库函数版)

	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//初始化定时器 14
	//初始化 TIM14 Channel1 PWM 模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM 调制模式 1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性低
	TIM_OC1Init(TIM5, &TIM_OCInitStructure); //初始化外设 TIM1 4OC1
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable); //使能预装载寄存器
	TIM_OC2Init(TIM5, &TIM_OCInitStructure); //初始化外设 TIM1 4OC1
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable); //使能预装载寄存器
	TIM_OC3Init(TIM5, &TIM_OCInitStructure); //初始化外设 TIM1 4OC1
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable); //使能预装载寄存器
	TIM_OC4Init(TIM5, &TIM_OCInitStructure); //初始化外设 TIM1 4OC1
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable); //使能预装载寄存器
	TIM_ARRPreloadConfig(TIM5,ENABLE);//ARPE 使能
	TIM_Cmd(TIM5, ENABLE); //使能 TIM14
}

void TIM12_PWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);//TIM14 时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE); //使能 PORTF 时钟
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE); //使能 PORTF 时钟
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource6,GPIO_AF_TIM12); //GF9 复用为 TIM14
//	GPIO_PinAFConfig(GPIOH,GPIO_PinSource11,GPIO_AF_TIM5); //GF9 复用为 TIM14
//	GPIO_PinAFConfig(GPIOH,GPIO_PinSource12,GPIO_AF_TIM5); //GF9 复用为 TIM14
//	GPIO_PinAFConfig(GPIOI,GPIO_PinSource0,GPIO_AF_TIM5); //GF9 复用为 TIM14
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12; //GPIOF9
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度 50MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
//	GPIO_Init(GPIOH,&GPIO_InitStructure); //初始化 PF9
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度 50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOH,&GPIO_InitStructure); //初始化 PF9
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc; //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr; //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//STM32F4 开发指南(库函数版)

	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//初始化定时器 14
	//初始化 TIM14 Channel1 PWM 模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM 调制模式 1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性低
	TIM_OC1Init(TIM12, &TIM_OCInitStructure); //初始化外设 TIM1 4OC1
	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable); //使能预装载寄存器
//	TIM_OC2Init(TIM5, &TIM_OCInitStructure); //初始化外设 TIM1 4OC1
//	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable); //使能预装载寄存器
//	TIM_OC3Init(TIM5, &TIM_OCInitStructure); //初始化外设 TIM1 4OC1
//	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable); //使能预装载寄存器
//	TIM_OC4Init(TIM5, &TIM_OCInitStructure); //初始化外设 TIM1 4OC1
//	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable); //使能预装载寄存器
	TIM_ARRPreloadConfig(TIM12,ENABLE);//ARPE 使能
//	TIM_Cmd(TIM12, DISABLE);
	TIM_Cmd(TIM12, ENABLE); //使能 TIM14
//	TIM_Cmd(TIM12, DISABLE); 
//	TIM_Cmd(TIM12, ENABLE); //使能 TIM14
//	TIM_Cmd(TIM12, DISABLE); 
}
