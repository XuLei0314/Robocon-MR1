#ifndef __REMOTE_CONTROL_H__
#define __REMOTE_CONTROL_H__

#include "main.h"

#define DBUS_BUF_SIZE    18
/* ----------------------- RC Channel Value Definition---------------------------- */
#define CH_VALUE_MIN                   ((uint16_t)364    ) 
#define CH_VALUE_OFFSET                ((uint16_t)1024   ) 
#define CH_VALUE_MAX                   ((uint16_t)1684   )  
/* ----------------------- RC Switch Value Definition----------------------------- */
#define SW_UP                          ((uint16_t)1      ) 
#define SW_MID                          ((uint16_t)3      ) 
#define SW_DOWN                        ((uint16_t)2      )  
/* ----------------------- PC Mouse Value Definition-------------------------------- */
#define MOUSE_MOVE_VALUE_MIN         ((uint16_t)-32768 ) 
#define MOUSE_MOVE_VALUE_OFFSET      ((uint16_t)0      ) 
#define MOUSE_MOVE_VALUE_MAX         ((uint16_t)32767  ) 
#define MOUSE_BTN_UP                  ((uint8_t)0       ) 
#define MOUSE_BTN_DN                  ((uint8_t)1       ) 
/* ----------------------- PC Key Value Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W         ((uint16_t)0x01<<0) 
#define KEY_PRESSED_OFFSET_S         ((uint16_t)0x01<<1) 
#define KEY_PRESSED_OFFSET_A         ((uint16_t)0x01<<2) 
#define KEY_PRESSED_OFFSET_D         ((uint16_t)0x01<<3) 
#define KEY_PRESSED_OFFSET_Q         ((uint16_t)0x01<<4) 
#define KEY_PRESSED_OFFSET_E         ((uint16_t)0x01<<5) 
#define KEY_PRESSED_OFFSET_SHIFT     ((uint16_t)0x01<<6) 
#define KEY_PRESSED_OFFSET_CTRL      ((uint16_t)0x01<<7)  
#define compass_eof 		0x77

#define  BSP_USART1_DMA_RX_BUF_LEN               30u                   
 
#define BSP_USART1_RX_BUF_SIZE_IN_FRAMES         (BSP_USART1_RX_BUF_SIZE / RC_FRAME_LENGTH)
#define  RC_FRAME_LENGTH                            18u


typedef struct
{
    struct
    {
        int16_t ch0;
        int16_t ch1;
        int16_t ch2;
        int16_t ch3;
        int8_t  s1;
        int8_t  s2;
    }rc;
    
    struct
    {
        int16_t x;
        int16_t y;
        int16_t z;
        int8_t l;
        int8_t r;
    }mouse;
    
    struct
    {
        int16_t v;
    }key;
    
    int16_t res;
    
}DBUS;
void Remote_CONTROL(void);
void DBUS_Enc(const DBUS* pdbus,unsigned char* pbuf);
void DBUS_Dec(DBUS* pdbus,const unsigned char* pbuf);
u8 UART1_Init(int BaudRate);

#endif
