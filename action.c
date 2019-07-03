#include "action.h" 

int flag=0;
extern float Action[6];
//extern float x1,y1,z1;
extern chassis_t chassis;
float x=0,y=0,z=0;

u8 UART6_Init(int BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); 
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOG,&GPIO_InitStructure); 
	
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
	USART_Init(USART6, &USART_InitStructure); 
	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//????? 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1; //????? 2
	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure); 
	
	USART_Cmd(USART6, ENABLE); 
	
return 0;
}
float pos_x=0;
float pos_y=0;
float zangle=0;
float xangle=0;
float yangle=0;
float w_z=0;
float Action[6];
int ReceiveBuffer_flag=0;
void USART6_IRQHandler(void)               
	{
    static uint8_t ch;
	static union
	{
		uint8_t data[24];
		float ActVal[6];
	}posture;
	static uint8_t count=0;
	static uint8_t i=0;
	
		if(USART_GetITStatus(USART6,USART_IT_RXNE) != RESET)
		{
			ReceiveBuffer_flag=1;
      USART_ClearITPendingBit(USART6, USART_IT_RXNE); 
			ch=USART_ReceiveData(USART6);
		
		switch(count)
		{
			case 0:
				if(ch==0x0d)
					count++;
				else
					count=0;
				break;
			case 1:
				if(ch==0x0a)
				{
					i=0;
					count++;
				}
				else if(ch==0x0d);
				else
				  count=0;
				break;
			case 2:
				posture.data[i]=ch;
			  i++;
			  if(i>=24)
				{
					i=0;
					count++;
				}
				break;
			case 3:
				if(ch==0x0a)
					count++;
				else
				  count=0;
				break;
			case 4:
				if(ch==0x0d)
				{	
					zangle=posture.ActVal[0];
					xangle=posture.ActVal[1];
					yangle=posture.ActVal[2];
					pos_x =posture.ActVal[3];
					pos_y =posture.ActVal[4];
					w_z =posture.ActVal[5];			
				}
				count=0;
				break;
			default:
				count=0;
			break;
		}
		}
		else
		{
			ReceiveBuffer_flag=0;
		}
		if(ReceiveBuffer_flag)
		{
			Action[0]=zangle;
			Action[1]=xangle;
			Action[2]=yangle;
			Action[3]=pos_x;
			Action[4]=pos_y;
			Action[5]=w_z;
		}
		else
		{
			Action[0]=Action[0];
			Action[1]=Action[1];
			Action[2]=Action[2];
			Action[3]=Action[3];
			Action[4]=Action[4];
			Action[5]=Action[5];
		}
	} 
	
void POSITON(int mode,int start,int end)
{//mode  1 x 2y 3z
	int direction;
	direction=end-start;
//	int i;
	if(mode==1)
	{//方向左 负  x<0 右 正  x>0
		if(direction<0)
		{
//   if(flag==0)
//	 {
			if(Action[3]<=start)
			{
				x=-10;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+10))&&(Action[3]>start))
			{
				x=-12;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+20))&&(Action[3]>(start+10)))
			{
				x=-14;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+30))&&(Action[3]>(start+20)))
			{
				x=-16;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+40))&&(Action[3]>(start+30)))
			{
				x=-18;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+50))&&(Action[3]>(start+40)))
			{
				x=-20;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+60))&&(Action[3]>(start+50)))
			{
				x=-22;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+70))&&(Action[3]>(start+60)))
			{
				x=-24;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+80))&&(Action[3]>(start+70)))
			{
				x=-26;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+90))&&(Action[3]>(start+80)))
			{
				x=-28;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+100))&&(Action[3]>(start+90)))
			{
				x=-30;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-100))&&(Action[3]>(start+100)))
			{
				x=-30;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-90))&&(Action[3]>(end-100)))
			{
				x=-28;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-80))&&(Action[3]>(end-90)))
			{
				x=-26;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-70))&&(Action[3]>(end-80)))
			{
				x=-24;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-60))&&(Action[3]>(end-70)))
			{
				x=-22;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-60))&&(Action[3]>(end-50)))
			{
				x=-20;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-40))&&(Action[3]>(end-50)))
			{
				x=-18;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-40))&&(Action[3]>(end-30)))
			{
				x=-16;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-30))&&(Action[3]>(end-20)))
			{
				x=-14;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-20))&&(Action[3]>(end-10)))
			{
				x=-12;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-10))&&(Action[3]>end))
			{
				x=-10;
				y=0;
				z=0;
			}
//			if(Action[3]==end)
//			{
//				flag=1;
//			}
//		}
	}
		if(direction>0)//反方向
		{
//			if(flag==2)
//			{
			if(Action[3]<=start)
			{
				x=10;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+10))&&(Action[3]>start))
			{
				x=12;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+20))&&(Action[3]>(start+10)))
			{
				x=14;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+30))&&(Action[3]>(start+20)))
			{
				x=16;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+40))&&(Action[3]>(start+30)))
			{
				x=18;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+50))&&(Action[3]>(start+40)))
			{
				x=20;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+60))&&(Action[3]>(start+50)))
			{
				x=22;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+70))&&(Action[3]>(start+60)))
			{
				x=24;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+80))&&(Action[3]>(start+70)))
			{
				x=26;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+90))&&(Action[3]>(start+80)))
			{
				x=28;
				y=0;
				z=0;
			}
			if((Action[3]<=(start+100))&&(Action[3]>(start+90)))
			{
				x=30;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-100))&&(Action[3]>(start+100)))
			{
				x=30;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-90))&&(Action[3]>(end-100)))
			{
				x=28;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-80))&&(Action[3]>(end-90)))
			{
				x=26;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-70))&&(Action[3]>(end-80)))
			{
				x=24;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-60))&&(Action[3]>(end-70)))
			{
				x=22;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-60))&&(Action[3]>(end-50)))
			{
				x=20;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-40))&&(Action[3]>(end-50)))
			{
				x=18;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-40))&&(Action[3]>(end-30)))
			{
				x=16;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-30))&&(Action[3]>(end-20)))
			{
				x=14;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-20))&&(Action[3]>(end-10)))
			{
				x=12;
				y=0;
				z=0;
			}
			if((Action[3]<=(end-10))&&(Action[3]>end))
			{
				x=10;
				y=0;
				z=0;
			}
//			if(Action[3]==end)
//			{
//				flag=3;
//			}
//		}
	}

	}
	
	
	if(mode==2)
	{
		if(direction>0)
		{
//			if(flag==3)
//			{
			if(Action[4]<=start)
			{
				x=0;
				y=-10;
				z=0;
			}
			if((Action[4]<=(start+10))&&(Action[4]>start))
			{
				x=0;
				y=-12;
				z=0;
			}
			if((Action[4]<=(start+20))&&(Action[4]>(start+10)))
			{
				x=0;
				y=-14;
				z=0;
			}
			if((Action[4]<=(start+30))&&(Action[4]>(start+20)))
			{
				x=0;
				y=-16;
				z=0;
			}
			if((Action[4]<=(start+40))&&(Action[4]>(start+30)))
			{
				x=0;
				y=-18;
				z=0;
			}
			if((Action[4]<=(start+50))&&(Action[4]>(start+40)))
			{
				x=0;
				y=-20;
				z=0;
			}
			if((Action[4]<=(start+60))&&(Action[4]>(start+50)))
			{
				x=0;
				y=-22;
				z=0;
			}
			if((Action[4]<=(start+70))&&(Action[4]>(start+60)))
			{
				x=0;
				y=-24;
				z=0;
			}
			if((Action[4]<=(start+80))&&(Action[4]>(start+70)))
			{
				x=0;
				y=-26;
				z=0;
			}
			if((Action[4]<=(start+90))&&(Action[4]>(start+80)))
			{
				x=0;
				y=-28;
				z=0;
			}
			if((Action[4]<=(start+100))&&(Action[4]>(start+90)))
			{
				x=0;
				y=-30;
				z=0;
			}
			if((Action[4]<=(end-100))&&(Action[4]>(start+100)))
			{
				x=0;
				y=-30;
				z=0;
			}
			if((Action[4]<=(end-90))&&(Action[4]>(end-100)))
			{
				x=0;
				y=-28;
				z=0;
			}
			if((Action[4]<=(end-80))&&(Action[4]>(end-90)))
			{
				x=0;
				y=-26;
				z=0;
			}
			if((Action[4]<=(end-70))&&(Action[4]>(end-80)))
			{
				x=0;
				y=-24;
				z=0;
			}
			if((Action[4]<=(end-60))&&(Action[4]>(end-70)))
			{
				x=0;
				y=-22;
				z=0;
			}
			if((Action[4]<=(end-60))&&(Action[4]>(end-50)))
			{
				x=0;
				y=-20;
				z=0;
			}
			if((Action[4]<=(end-40))&&(Action[4]>(end-50)))
			{
				x=0;
				y=-18;
				z=0;
			}
			if((Action[4]<=(end-40))&&(Action[4]>(end-30)))
			{
				x=0;
				y=-16;
				z=0;
			}
			if((Action[4]<=(end-30))&&(Action[4]>(end-20)))
			{
				x=0;
				y=-14;
				z=0;
			}
			if((Action[4]<=(end-20))&&(Action[4]>(end-10)))
			{
				x=0;
				y=-12;
				z=0;
			}
			if((Action[4]<=(end-10))&&(Action[4]>end))
			{
				x=0;
				y=-10;
				z=0;
			}
//			if(Action[4]==end)
//			{
//				flag=4;
//			}
//		}
		}
		if(direction<0)//反方向
		{
//			if(flag==1)
//			{
			if(Action[4]<=start)
			{
				x=0;
				y=10;
				z=0;
			}
			if((Action[4]<=(start+10))&&(Action[4]>start))
			{
				x=0;
				y=12;
				z=0;
			}
			if((Action[4]<=(start+20))&&(Action[4]>(start+10)))
			{
				x=0;
				y=14;
				z=0;
			}
			if((Action[4]<=(start+30))&&(Action[4]>(start+20)))
			{
				x=0;
				y=16;
				z=0;
			}
			if((Action[4]<=(start+40))&&(Action[4]>(start+30)))
			{
				x=0;
				y=18;
				z=0;
			}
			if((Action[4]<=(start+50))&&(Action[4]>(start+40)))
			{
				x=0;
				y=20;
				z=0;
			}
			if((Action[4]<=(start+60))&&(Action[4]>(start+50)))
			{
				x=0;
				y=22;
				z=0;
			}
			if((Action[4]<=(start+70))&&(Action[4]>(start+60)))
			{
				x=0;
				y=24;
				z=0;
			}
			if((Action[4]<=(start+80))&&(Action[4]>(start+70)))
			{
				x=0;
				y=26;
				z=0;
			}
			if((Action[4]<=(start+90))&&(Action[4]>(start+80)))
			{
				x=0;
				y=28;
				z=0;
			}
			if((Action[4]<=(start+100))&&(Action[4]>(start+90)))
			{
				x=0;
				y=30;
				z=0;
			}
			if((Action[4]<=(end-100))&&(Action[4]>(start+100)))
			{
				x=0;
				y=30;
				z=0;
			}
			if((Action[4]<=(end-90))&&(Action[4]>(end-100)))
			{
				x=0;
				y=28;
				z=0;
			}
			if((Action[4]<=(end-80))&&(Action[4]>(end-90)))
			{
				x=0;
				y=26;
				z=0;
			}
			if((Action[4]<=(end-70))&&(Action[4]>(end-80)))
			{
				x=0;
				y=24;
				z=0;
			}
			if((Action[4]<=(end-60))&&(Action[4]>(end-70)))
			{
				x=0;
				y=22;
				z=0;
			}
			if((Action[4]<=(end-60))&&(Action[4]>(end-50)))
			{
				x=0;
				y=20;
				z=0;
			}
			if((Action[4]<=(end-40))&&(Action[4]>(end-50)))
			{
				x=0;
				y=18;
				z=0;
			}
			if((Action[4]<=(end-40))&&(Action[4]>(end-30)))
			{
				x=0;
				y=16;
				z=0;
			}
			if((Action[4]<=(end-30))&&(Action[4]>(end-20)))
			{
				x=0;
				y=14;
				z=0;
			}
			if((Action[4]<=(end-20))&&(Action[4]>(end-10)))
			{
				x=0;
				y=12;
				z=0;
			}
			if((Action[4]<=(end-10))&&(Action[4]>end))
			{
				x=0;
				y=10;
				z=0;
			}
//			if(Action[4]==end)
//			{
//				flag=2;
//			}
//		}
		}
//		Action[4];
	}
	if(mode==3)
	{
	}
}
extern int time;
extern int flag2;

void action(void)
{
//	if(flag2<2)
//	{
//  if(time<15)
//	{
//		TIM_SetCompare1(TIM12,25000);
//	}
//	else
//	{
//		TIM_SetCompare1(TIM12,0);
//	}
//}
	if(!Pause_Key)
	{

		if(flag==0)
			{
	if((Action[3]>-30)&&(Action[3]<=100))
			{
				y=0;
				x=-5;
				z=0;
			}
			if((Action[3]>100)&&(Action[3]<=200))
			{
				y=0;
				z=0;
				x=-10;
			}
			if((Action[3]>200)&&(Action[3]<=900))
			{
				y=0;
				z=0;
				x=-15;
			}
      if((Action[3]>900)&&(Action[3]<=1100))
			{
				y=0;
				z=0;
				x=-10;
			}
			if((Action[3]>1100)&&(Action[3]<=1300))
			{
				y=0;
				z=0;
				x=-7;
			}
			if((Action[3]>1300)&&(Action[3]<1400))
			{
				y=0;
				z=0;
				x=-5;
			}
			if(Action[3]>=1400)
			{
				flag=1;
				x=0;
				y=0;
				z=0;

			}
		}
			if(flag==1)
			{
				
			if((Action[4]<30)&&(Action[4]>=-100))
			{
				
//    count1=1;
			  y=-5;
	      z=0;
				x=0;
//	      x=5;
			}
			if((Action[4]<-100)&&(Action[4]>=-200))
			{
				//GREEN_ON;
//    count1=1;
			  y=-15;
	      z=0;
				x=0;
//	      x=0;
			}
			if((Action[4]<-200)&&(Action[4]>=-1700))
			{
				//GREEN_ON;
//    count1=1;
			  y=-30;
				x=0;
	      z=0;
//	      x=0;
			}
			if((Action[4]<-1700)&&(Action[4]>=-1800))
			{
				//GREEN_ON;
//    count1=1;
			  y=-20;
	      z=0;
				x=0;
//	      x=0;
			}
			if((Action[4]<-1800)&&(Action[4]>=-1900))
			{
				//GREEN_ON;
//    count1=1;
			  y=-15;
	      z=0;
				x=0;
//	      x=0;
			}
			if((Action[4]<-1900)&&(Action[4]>=-2000))
			{
				//GREEN_ON;
//    count1=1;
			  y=-10;
	      z=0;
				x=0;
//	      x=0;
			}
			if((Action[4]<-2000)&&(Action[4]>=-2100))
			{
				//GREEN_ON;
//    count1=1;
			  y=-7;
	      z=0;
				x=0;
//	      x=0;
			}
			if((Action[4]<-2100)&&(Action[4]>=-2200))
			{
				//GREEN_ON;
//    count1=1;
			  y=-5;
	      z=0;
				x=0;
//	      x=0;
			}
			if((Action[4]<-2200)&&(Action[4]>=-2300))
			{
			//	GREEN_ON;
//    count1=1;
			  y=-5;
	      z=0;
				x=0;
//	      x=0;
			}
			if(Action[4]<-2300)
			{
			//	GREEN_OFF;
//				count1=0;
				flag=2;
//				x=0;
			}
		}
			if(flag==2)
			{
				if(Action[3]<=50)
			{
				flag=3;
			}
			if((Action[3]<50)&&(Action[3])<=100)
			{
				y=0;
				x=3;
				z=0;
			}
				if((Action[3]>100)&&(Action[3]<=300))
			{
				y=0;
				x=5;
				z=0;
			}
			if((Action[3]>300)&&(Action[3]<=400))
			{
				y=0;
				z=0;
				x=7;
			}
			if((Action[3]>400)&&(Action[3]<=500))
			{
				y=0;
				z=0;
				x=10;
			}
			if((Action[3]>500)&&(Action[3]<=1300))
			{
				y=0;
				z=0;
				x=15;
			}
			if((Action[3]>1300)&&(Action[3]<1400))
			{
				y=0;
				z=0;
				x=10;
			}
			if(Action[3]>=1400)
			{
				y=0;
				z=0;
				x=5;

			}
			}
			if(flag==3)
			{
				if((Action[4]<=-2000)&&(Action[4]>=-2500))
			{
				GREEN_ON;
			  y=-5;
	      z=0;
				x=0;
			}
//			if((Action[4]<-2400)&&(Action[4]<=-2500))
//			{
//				GREEN_ON;
//			  y=-5;
//	      z=0;
//				x=0;
//			}
		  if((Action[4]<-2500)&&(Action[4]<=-2600))
			{
				GREEN_ON;
			  y=-10;
	      z=0;
				x=0;
			}
			if((Action[4]<-2600)&&(Action[4]<=-2700))
			{
				GREEN_ON;
			  y=-20;
	      z=0;
				x=0;
			}
			if((Action[4]<-2700)&&(Action[4]<=-4700))
			{
				GREEN_ON;
			  y=-30;
	      z=0;
				x=0;
			}
			if((Action[4]<-4700)&&(Action[4]<=-4800))
			{
				GREEN_ON;
			  y=-20;
	      z=0;
				x=0;
			}
			if((Action[4]<-4800)&&(Action[4]<=-4900))
			{
				GREEN_ON;
			  y=-10;
	      z=0;
				x=0;
			}
			
				if((Action[4]<-4900)&&(Action[4]<=-5000))
			{
				GREEN_ON;
			  y=-7;
	      z=0;
				x=0;
			}
			if((Action[4]<-5000)&&(Action[4]<=-5100))
			{
				GREEN_ON;
			  y=-5;
	      z=0;
				x=0;
			}
			if((Action[4]<-5100)&&(Action[4]<=-5200))
			{
				GREEN_ON;
			  y=-3;
	      z=0;
				x=0;
			}
			if(Action[4]<=-5200)
			{
				GREEN_ON;
			  y=0;
	      z=0;
				x=0;
			}
			}
		}
		else
		{
			GREEN_OFF;
			y=0;
			x=0;
			z=0;
		}
 
		chassis.vy = y; 
		chassis.vw =z;
		chassis.vx =x;

		}
