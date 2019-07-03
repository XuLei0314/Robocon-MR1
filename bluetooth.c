#include "bluetooth.h"

u8 UART2_Init(int BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOD,&GPIO_InitStructure); 
	
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
	USART_Init(USART2, &USART_InitStructure); 
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//????? 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1; //????? 2
	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure); 
	
	USART_Cmd(USART2, ENABLE); 
	
return 0;
}
const char sof = 0xA5;
const char end = 0xFE;
char RECIVE_start=0;
volatile char rec_buff[30];
volatile int rec_count=0;
char temp_buf[14]={0};
u16 Usart6_time_cnt = 0;
tReceTXoneData ReceData = {0};
//extern tReceTXoneData ReceData;
void USART2_IRQHandler(void)
{
if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
		{
			USART_ClearITPendingBit( USART2, USART_FLAG_RXNE);
//			//LED_ON;
//			temp_buf[rec_count] = USART_ReceiveData(USART2);
//			if(temp_buf[rec_count] == sof)
//			{
//				RECIVE_start = 1;
//				rec_count = 1;
//				return;
//			}
//			if(RECIVE_start)
//			{
//				temp_buf[rec_count++] = USART_ReceiveData(USART2);
//				
//				if(temp_buf[rec_count-1] == end)
//				{
//					RECIVE_start = 0;
//					rec_count = 0;
//					
//					memcpy(&ReceData,temp_buf,sizeof(tReceTXoneData));
//					//LED_OFF;
//				}
//			}
//			delay_ms(2);
		}
}
u8 SV1_State = 0x0000;
u8 SV2_State = 0x0000;
u8 SV3_State = 0x0000;
u8 SV4_State = 0x0000;
u8 SV5_State = 0x0000;
u8 SV6_State = 0x0000;
u8 SV7_State = 0x0000;
u8 SV8_State = 0x0000;
uint8_t auto_send1[50];
tSendTXoneData SendData = {0};
void bluetooth_send(void)
{
	    
//		SendData.sof1=0xA4;
//		SendData.SEND1_1=0x0000;
//		SendData.SEND2_1=0x0000;
//		SendData.SEND3_1=0x0000;
//		SendData.SEND4_1=0x0000;
//		SendData.SEND5_1=0x0000;
//		SendData.SEND6_1=0x0000;
//		SendData.SEND7_1=0x0000;
//		SendData.SEND8_1=0x0000;
//		SendData.end1=0xF1;
//		memcpy(auto_send1, &SendData, sizeof(tSendTXoneData));
//		for(t=0;t<sizeof(tSendTXoneData);t++)
//		{
////		    GREEN_ON;
//			USART_SendData(USART2,auto_send1[t]);
//			while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
//		}

}
