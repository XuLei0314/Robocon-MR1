#include "remote_control.h"

unsigned char dbus_buf[DBUS_BUF_SIZE];
int rc_get;
DBUS dbus;
int buf[6];

u8 UART1_Init(int BaudRate)
{
	 GPIO_InitTypeDef gpio;
	  USART_InitTypeDef usart;
	  NVIC_InitTypeDef nvic;
    DMA_InitTypeDef dma;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
    
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
    
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin = GPIO_Pin_7;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &gpio);
    
    USART_DeInit(USART1);
    USART_StructInit(&usart);
    usart.USART_BaudRate = BaudRate;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_Even;
    usart.USART_Mode = USART_Mode_Rx;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &usart);
    
  
    USART_Cmd(USART1, ENABLE);
		USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    
		nvic.NVIC_IRQChannel = DMA2_Stream5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
		
    DMA_DeInit(DMA2_Stream5);
    dma.DMA_Channel= DMA_Channel_4;
    dma.DMA_PeripheralBaseAddr = (u32)&(USART1->DR);
    dma.DMA_Memory0BaseAddr = (u32)dbus_buf;
    dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
    dma.DMA_BufferSize = DBUS_BUF_SIZE;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_Priority = DMA_Priority_VeryHigh;
    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    dma.DMA_MemoryBurst = DMA_Mode_Normal;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream5,&dma);

    DMA_ITConfig(DMA2_Stream5,DMA_IT_TC,ENABLE);
    DMA_Cmd(DMA2_Stream5,ENABLE);
	
	return 0;
}

void DMA2_Stream5_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA2_Stream5, DMA_IT_TCIF5))
    {
       
        DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5);
        DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TCIF5);

        /*******************decode DBUS data*******************/
        DBUS_Dec(&dbus,dbus_buf);
			buf[0]=dbus.rc.ch0;
			buf[1]=dbus.rc.ch1;
			buf[2]=dbus.rc.ch2;
			buf[3]=dbus.rc.ch3;
			buf[4]=dbus.rc.s1;
			buf[5]=dbus.rc.s2;
        
    }
}
void DBUS_Enc(const DBUS* pdbus,unsigned char* pbuf)
{
    pbuf[0] = pdbus->rc.ch0&0xff;
    pbuf[1] = (pdbus->rc.ch1<<3) | (pdbus->rc.ch0>>8);
    pbuf[2] = (pdbus->rc.ch2<<6) | (pdbus->rc.ch1>>5);
    pbuf[3] = pdbus->rc.ch2;
    pbuf[4] = (pdbus->rc.ch3<<1) | (pdbus->rc.ch2>>10);
    pbuf[5] = (pdbus->rc.s2<<6) | (pdbus->rc.s1<<4) | (pdbus->rc.ch3>>7);
    pbuf[6] = pdbus->mouse.x;
    pbuf[7] = pdbus->mouse.x>>8;
    pbuf[8] = pdbus->mouse.y;
    pbuf[9] = pdbus->mouse.y>>8;
    pbuf[10] = pdbus->mouse.z;
    pbuf[11] = pdbus->mouse.z>>8;
    pbuf[12] = pdbus->mouse.l;
    pbuf[13] = pdbus->mouse.r;
    pbuf[14] = pdbus->key.v;
    pbuf[15] = pdbus->key.v>>8;
    pbuf[16] = pdbus->res;
    pbuf[17] = pdbus->res>>8;
}


void DBUS_Dec(DBUS* pdbus,const unsigned char* pbuf)
{
	if(!rc_get && ((pbuf[16] | (pbuf[17] << 8))-1024) == 0 )
	{
		rc_get = 1;
		//RED_ON;
		printf("rc_get!\r\n");
	}
	
    pdbus->rc.ch0 = (pbuf[0] | (pbuf[1] << 8)) & 0x07ff;          //!< Channel 0  
		pdbus->rc.ch0-=1024;
    pdbus->rc.ch1 = ((pbuf[1] >> 3) | (pbuf[2] << 5)) & 0x07ff;   //!< Channel 1         
    pdbus->rc.ch1-=1024;
		pdbus->rc.ch2 = ((pbuf[2] >> 6) | (pbuf[3] << 2) |            //!< Channel 2                          
        (pbuf[4] << 10)) & 0x07ff;
		pdbus->rc.ch2-=1024;
    pdbus->rc.ch3 = ((pbuf[4] >> 1) | (pbuf[5] << 7)) & 0x07ff;   //!< Channel 3
		pdbus->rc.ch3-=1024;	
		pdbus->rc.s1 = ((pbuf[5] >> 4) & 0x000C) >> 2;                    //!< Switch left         
    pdbus->rc.s2 = ((pbuf[5] >> 4) & 0x0003);                         //!< Switch right  
    pdbus->mouse.x = pbuf[6] | (pbuf[7] << 8);                    //!< Mouse X axis 
    pdbus->mouse.y = pbuf[8] | (pbuf[9] << 8);                    //!< Mouse Y axis 
    pdbus->mouse.z = pbuf[10] | (pbuf[11] << 8);                  //!< Mouse Z axis 
    pdbus->mouse.l = pbuf[12];                                        //!< Mouse Left Is Press ?
    pdbus->mouse.r = pbuf[13];                                        //!< Mouse Right Is Press ? 
    pdbus->key.v = pbuf[14] | (pbuf[15] << 8);                    //!< KeyBoard value   
    pdbus->res = pbuf[16] | (pbuf[17] << 8);                      //!< Reserve 
	  pdbus->res -= 1024;
}
extern u8 SV1_State;
extern u8 SV2_State;
extern u8 SV3_State;
extern u8 SV4_State;
extern u8 SV5_State;
extern u8 SV6_State;
extern u8 SV7_State;
extern u8 SV8_State;
float x2,y2,z2;
void Remote_CONTROL(void)
{
//		rc_get=1;
//		if((buf[0]<125)&&(buf[0]>20))
//		{
//			x2=buf[0]/10;
//		}
//		else if((buf[0]<-20)&&(buf[0]>-125))
//		{
//			x2=buf[0]/10;
//		}
////		if((buf[0]>=50)&&(buf[0]<85))
////		{
////			x=1;
////		} 
////		else if((buf[0]>85)&&(buf[0]<125))
////		{
////			x=2;
////		} 
//		else if((buf[0]>=125)&&(buf[0]<160))
//		{
//			x2=3;
//		}
//		else if((buf[0]>=160)&&(buf[0]<200))
//		{
//			x2=4;
//		}
//		else if((buf[0]>=200)&&(buf[0]<275))
//		{
//			x2=6;
//		}
//		else if((buf[0]>=275)&&(buf[0]<350))
//		{
//			x2=8;
//		}
//		else if((buf[0]>=350)&&(buf[0]<425))
//		{
//			x2=10;
//		}
//		else if((buf[0]>=425)&&(buf[0]<500))
//		{
//			x2=14;
//		}
//		else if((buf[0]>=500)&&(buf[0]<=575))
//		{
//			x2=18;
//		}
//		else if((buf[0]>=575)&&(buf[0]<=660))
//		{
//			x2=22;
//		}
////		else if((buf[0]>-85)&&(buf[0]<=-50))
////		{
////			x=-1;
////		}
////		else if((buf[0]>-125)&&(buf[0]<=-85))
////		{
////			x=-2;
////		}
//		else if((buf[0]>-160)&&(buf[0]<=-125))
//		{
//			x2=-3;
//		}
//		else if((buf[0]>-200)&&(buf[0]<=-160))
//		{
//			x2=-4;
//		}
//		else if((buf[0]>-275)&&(buf[0]<=-200))
//		{
//			x2=-6;
//		}
//		else if((buf[0]>-350)&&(buf[0]<=-275))
//		{
//			x2=-8;
//		}
//		else if((buf[0]>-425)&&(buf[0]<=-350))
//		{
//			x2=-10;
//		}
//		else if((buf[0]>-500)&&(buf[0]<=-425))
//		{
//			x2=-14;
//		}
//		else if((buf[0]>=-575)&&(buf[0]<=-500))
//		{
//			x2=-18;
//		}
//		else if((buf[0]>=-660)&&(buf[0]<=-575))
//		{
//			x2=-22;
//		}
//		else
//		{
//			x2=0;
//		}	
//		
//		if((buf[1]<125)&&(buf[1]>20))
//		{
//			y2=-buf[0]/10;
//		}
//		else if((buf[1]<-20)&&(buf[1]>125))
//		{
//			y2=-buf[0]/10;
//		}
////		if((buf[1]>=50)&&(buf[1]<85))
////		{
////			y=-1;
////		} 
////		else if((buf[1]>85)&&(buf[1]<125))
////		{
////			y=-2;
////		} 
//		else if((buf[1]>=125)&&(buf[1]<160))
//		{
//			y2=-3;
//		}
//		else if((buf[1]>=160)&&(buf[1]<200))
//		{
//			y2=-4;
//		}
//		else if((buf[1]>=200)&&(buf[1]<275))
//		{
//			y2=-6;
//		}
//		else if((buf[1]>=275)&&(buf[1]<350))
//		{
//			y2=-8;
//		}
//		else if((buf[1]>=350)&&(buf[1]<425))
//		{
//			y2=-10;
//		}
//		else if((buf[1]>=425)&&(buf[1]<500))
//		{
//			y2=-14;
//		}
//		else if((buf[1]>=500)&&(buf[1]<=575))
//		{
//			y2=-18;
//		}
//		else if((buf[1]>=575)&&(buf[1]<=660))
//		{
//			y2=-22;
//		}
////		else if((buf[1]>-85)&&(buf[1]<=-50))
////		{
////			y=1;
////		}
////		else if((buf[1]>-125)&&(buf[1]<=-85))
////		{
////			y=2;
////		}
//		else if((buf[1]>-160)&&(buf[1]<=-125))
//		{
//			y2=3;
//		}
//		else if((buf[1]>-200)&&(buf[1]<=-160))
//		{
//			y2=4;
//		}
//		else if((buf[1]>-275)&&(buf[1]<=-200))
//		{
//			y2=6;
//		}
//		else if((buf[1]>-350)&&(buf[1]<=-275))
//		{
//			y2=8;
//		}
//		else if((buf[1]>-425)&&(buf[1]<=-350))
//		{
//			y2=10;
//		}
//		else if((buf[1]>-500)&&(buf[1]<=-425))
//		{
//			y2=14;
//		}
//		else if((buf[1]>=-575)&&(buf[1]<=-500))
//		{
//			y2=18;
//		}
//		else if((buf[1]>=-660)&&(buf[1]<=-575))
//		{
//			y2=22;
//		}
//		else
//		{
//			y2=0;
//		}	
//		
//		if((buf[2]<125)&&(buf[2]>20))
//		{
//			z2=buf[0]/10;
//		}
//		else if((buf[2]<-20)&&(buf[2]>-125))
//		{
//			z2=buf[0]/10;
//		}
////		if((buf[2]>=50)&&(buf[2]<85))
////		{
////			z=1;
////		} 
////		else if((buf[2]>85)&&(buf[2]<125))
////		{
////			z=2;
////		} 
//		else if((buf[2]>=125)&&(buf[2]<160))
//		{
//			z2=3;
//		}
//		else if((buf[2]>=160)&&(buf[2]<200))
//		{
//			z2=4;
//		}
//		else if((buf[2]>=200)&&(buf[2]<275))
//		{
//			z2=6;
//		}
//		else if((buf[2]>=275)&&(buf[2]<350))
//		{
//			z2=8;
//		}
//		else if((buf[2]>=350)&&(buf[2]<425))
//		{
//			z2=10;
//		}
//		else if((buf[2]>=425)&&(buf[2]<500))
//		{
//			z2=14;
//		}
//		else if((buf[2]>=500)&&(buf[2]<=575))
//		{
//			z2=18;
//		}
//		else if((buf[2]>=575)&&(buf[2]<=660))
//		{
//			z2=22;
//		}
////		else if((buf[2]>-85)&&(buf[2]<=-50))
////		{
////			z=-1;
////		}
////		else if((buf[2]>-125)&&(buf[2]<=-85))
////		{
////			z=-2;
////		}
//		else if((buf[2]>-160)&&(buf[2]<=-125))
//		{
//			z2=-3;
//		}
//		else if((buf[2]>-200)&&(buf[2]<=-160))
//		{
//			z2=-4;
//		}
//		else if((buf[2]>-275)&&(buf[2]<=-200))
//		{
//			z2=-6;
//		}
//		else if((buf[2]>-350)&&(buf[2]<=-275))
//		{
//			z2=-8;
//		}
//		else if((buf[2]>-425)&&(buf[2]<=-350))
//		{
//			z2=-10;
//		}
//		else if((buf[2]>-500)&&(buf[2]<=-425))
//		{
//			z2=-14;
//		}
//		else if((buf[2]>=-575)&&(buf[2]<=-500))
//		{
//			z2=-18;
//		}
//		else if((buf[2]>=-660)&&(buf[2]<=-575))
//		{
//			z2=-22;
//		}
//		else
//		{
//			z2=0;
//		}	
//		
//		
////		TIM_SetCompare1(TIM5,1880); //ÁîÅÆ¼Ð½ô1Ð¡ 2 ´ó×ó2 ÓÒ1    1  2  1750  1910   1  1880  1780 
////		TIM_SetCompare2(TIM5,1720);//1950  1785 1780 1710
////		TIM_SetCompare3(TIM5,1880);//1950  1785 1780 1710
////		TIM_SetCompare1(TIM5,1940); //ÁîÅÆ¼Ð½ô1Ð¡ 2 ´ó
////		TIM_SetCompare2(TIM5,1790);   ×¥  1830  1880   1910
////		//´ó¶æ»ú1790-1940
////		//Ð¡¶æ»ú1730-1950	
//		if((buf[4]==1)&&(buf[5]==1))//¼ÐÁîÅÆ
//		{
//			TIM_SetCompare1(TIM5,1750); //¹Õ¹Ç·­×ª
//			TIM_SetCompare2(TIM5,1910); //¹Õ¹Ç·­×ª
//			TIM_SetCompare3(TIM5,1850); //×¥È¡¹Õ¹Ç
//			TIM_SetCompare4(TIM5,1812); //
//			SV5_State=0x0000;
//			SV6_State=0x0000;
//			SV7_State=0x0000;
//			SV8_State=0x0000;
//		}
//		if((buf[4]==1)&&(buf[5]==3))//ËÉÁîÅÆ
//		{
//			TIM_SetCompare1(TIM5,1750); //ÁîÅÆËÉ¿ª
//			TIM_SetCompare2(TIM5,1910); //¼Ð½ô¹Õ¹Ç
//			TIM_SetCompare3(TIM5,1850); //¹Õ¹Ç·­×ª
//			TIM_SetCompare4(TIM5,1900); //¹Õ¹Ç·­×ª
//			SV5_State=0x0000;
//			SV6_State=0x0000;
//			SV7_State=0x0000;
//			SV8_State=0x0000;
//		}
//		if((buf[4]==1)&&(buf[5]==2))//¼Ð¹Õ¹Ç Æø¸×ÂäÏÂ
//		{
//			TIM_SetCompare1(TIM5,1750); //ÁîÅÆËÉ¿ª
//			TIM_SetCompare2(TIM5,1910); //¼Ð½ô¹Õ¹Ç
//			TIM_SetCompare3(TIM5,1910); //¹Õ¹Ç·­×ª
//			TIM_SetCompare4(TIM5,1900); //¹Õ¹Ç·­×ª
//			SV5_State=0x0001;
//			SV6_State=0x0000;
//			SV7_State=0x0000;
//			SV8_State=0x0000;
//		}
//		if((buf[4]==3)&&(buf[5]==2))//·­×ª 
//		{
//			TIM_SetCompare1(TIM5,1880); //ÁîÅÆËÉ¿ª
//			TIM_SetCompare2(TIM5,1780); //¼Ð½ô¹Õ¹Ç
//			TIM_SetCompare3(TIM5,1910); //¹Õ¹Ç·­×ª
//			TIM_SetCompare4(TIM5,1900); //¹Õ¹Ç·­×ª
//			SV5_State=0x0001;
//			SV6_State=0x0000;
//			SV7_State=0x0000;
//			SV8_State=0x0000;
//		}
//		if((buf[4]==2)&&(buf[5]==2))//Æø¸×Éì³ö
//		{
//			TIM_SetCompare1(TIM5,1880); //ÁîÅÆËÉ¿ª
//			TIM_SetCompare2(TIM5,1780); //¼Ð½ô¹Õ¹Ç
//			TIM_SetCompare3(TIM5,1910); //¹Õ¹Ç·­×ª
//			TIM_SetCompare4(TIM5,1900); //¹Õ¹Ç·­×ª
//			SV5_State=0x0001;
//			SV6_State=0x0000;
//			SV7_State=0x0000;
//			SV8_State=0x0001;
//		}
//		if((buf[4]==2)&&(buf[5]==3))//ËÉ¿ª¹Õ¹Ç
//		{
//			TIM_SetCompare1(TIM5,1880); //ÁîÅÆËÉ¿ª
//			TIM_SetCompare2(TIM5,1780); //¼Ð½ô¹Õ¹Ç
//			TIM_SetCompare3(TIM5,1830); //¹Õ¹Ç·­×ª
//			TIM_SetCompare4(TIM5,1900); //¹Õ¹Ç·­×ª
//			SV5_State=0x0001;
//			SV6_State=0x0000;
//			SV7_State=0x0000;
//			SV8_State=0x0001;
//		}
//		if((buf[4]==2)&&(buf[5]==1))//·¢Éä
//		{
//			TIM_SetCompare1(TIM5,1880); //ÁîÅÆËÉ¿ª
//			TIM_SetCompare2(TIM5,1780); //¼Ð½ô¹Õ¹Ç
//			TIM_SetCompare3(TIM5,1830); //¹Õ¹Ç·­×ª
//			TIM_SetCompare4(TIM5,1900); //¹Õ¹Ç·­×ª
//			SV5_State=0x0001;
//			SV6_State=0x0000;
//			SV7_State=0x0001;
//			SV8_State=0x0001;
//		}

}
