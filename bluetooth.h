#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__


#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
//#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	typedef __packed struct
{
	uint8_t sof;      //0xA5
//	int16_t angle;    //chassis angle
  uint16_t temp1_1;      //chassis angluar rate  deg/s
	uint16_t temp2_1;      //x velocity  mm/s
	uint16_t temp3_1;      //y velocity  mm/s
  uint16_t temp4_1;
	uint16_t temp5_1;
	uint16_t temp6_1;
	uint16_t temp7_1;      //y velocity  mm/s
	uint16_t temp8_1;

	uint8_t end;      //0xFE
} tReceTXoneData;

	typedef __packed struct
{
	uint8_t sof1;      //0xA5
  uint16_t SEND1_1;      //chassis angluar rate  deg/s
	uint16_t SEND2_1;      //x velocity  mm/s
	uint16_t SEND3_1;      //y velocity  mm/s
  uint16_t SEND4_1;
	uint16_t SEND5_1;
	uint16_t SEND6_1;
	uint16_t SEND7_1;      //y velocity  mm/s
	uint16_t SEND8_1;
	uint8_t end1;      //0xFE
} tSendTXoneData;

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
u8 UART2_Init(int BaudRate);
void bluetooth_send(void);

#endif
