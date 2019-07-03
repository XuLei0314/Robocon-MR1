/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;
moto_measure_t moto_chassis[4] = { 0 } ;
chassis_t chassis = { 0 };
extern pid_t pid_spd[4];
extern u16 ppm_rx[10];
volatile float Gyno;
//float x1=0,y1=0,z1=0;
//extern float x2,y2,z2;
//char bt_flag = 0;
/* Private function prototypes -----------------------------------------------*/
//static void Delay(__IO uint32_t nTime);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

int fputc(int ch, FILE *f)
{      
	while((USART6->SR&0X40)==0);//循环发送,直到发送完毕   
    USART6->DR = (u8) ch;      
	return ch;
}

int main(void)
{	
	int j;	
	BSP_Init();
	for(j = 0;j < 4;j++)
	  PID_struct_init(&pid_spd[j], POSITION_PID, 11000, 1500, 18.0f, 1.0f, 1.0f);
	SV1_ON;
	SV2_ON;
	SV3_ON;
	while (1)
	{
		action();
//  Remote_CONTROL();
	}
}
