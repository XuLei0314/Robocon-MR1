#ifndef __BASE_H__
#define __BASE_H__

#include "stm32f4xx.h"
#include "sys.h"
#define RED_ON  	GPIOE->BSRRH = GPIO_Pin_11
#define RED_OFF 	GPIOE->BSRRL = GPIO_Pin_11

#define GREEN_ON 	GPIOF->BSRRH = GPIO_Pin_14
#define GREEN_OFF GPIOF->BSRRL = GPIO_Pin_14
#define LED_ON 	 GPIOG->BSRRH = GPIO_Pin_1
#define LED_OFF  GPIOG->BSRRL = GPIO_Pin_1
#define KEY1		    (GPIOB->IDR & GPIO_Pin_2)
//#define KEY2			(GPIOB->IDR & GPIO_Pin_4)

#define SV1_OFF		GPIOH->BSRRH = GPIO_Pin_2
#define SV1_ON		GPIOH->BSRRL = GPIO_Pin_2

#define SV2_OFF		GPIOH->BSRRH = GPIO_Pin_3
#define SV2_ON		GPIOH->BSRRL = GPIO_Pin_3

#define SV3_OFF		GPIOH->BSRRH = GPIO_Pin_4
#define SV3_ON		GPIOH->BSRRL = GPIO_Pin_4

#define SV4_OFF		GPIOH->BSRRH = GPIO_Pin_5
#define SV4_ON   GPIOH->BSRRL = GPIO_Pin_5

//#define SV5_ON		GPIOB->BSRRH = GPIO_Pin_11
//#define SV5_OFF  	GPIOB->BSRRL = GPIO_Pin_11

#define LED0      PIout(0)

void Pause_Key_Init(void);
void power_Init(void);
u8 LED_KEY_Init(void);
u8 SV_Init(void);


#endif
