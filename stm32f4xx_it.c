/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "stm32f4xx_it.h"
#include "main.h"
#include "string.h"
#include "delay.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}


//void USART1_IRQHandler(void)                	//串口1中断服务程序
//	{

//	
//		if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
//		{

//		}
//	} 

//char temp_buf[30];	
//volatile int rec_count=0;
//char RECIVE_start = 0;
//char data_size = 0;
//extern volatile float Gyno;
//u16  rec_sum = 0;
//void USART3_IRQHandler(void)                	//串口1中断服务程序
//{		
//		int i;
//		if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)
//		{
//			
//		//	USART1->DR = USART_ReceiveData(USART3);
//			temp_buf[rec_count] = USART_ReceiveData(USART3);
//			if((temp_buf[rec_count] == compass_eof) && (!RECIVE_start))
//			{
//				RECIVE_start = 1;
//				rec_count = 1;
//				return;
//			}
//			if(RECIVE_start)
//			{
//				temp_buf[rec_count++] = USART_ReceiveData(USART3);
//				data_size = temp_buf[1];
//			//	printf("data_size is %d\n",data_size);
//				if((rec_count-1) == data_size)
//				{
//					rec_sum = 0;
//					for(i = 1;i<data_size;i++)
//					{
//						rec_sum+=temp_buf[i];
////							while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
////									USART1->DR = temp_buf[i];      
//					}
//					
//					
//						//USART1->DR = rec_sum&0x00ff;
//						
//					//	USART1->DR = 0xFF;
//					
//						//USART1->DR = temp_buf[12];
//					//	printf("sum is %d,temp is %d \n",rec_sum&0xff,temp_buf[data_size]);
//						
//					 if(temp_buf[data_size] == (rec_sum&0x00ff) )
//					 {
//							Gyno = (-1)*(temp_buf[data_size-3]>>4)*((temp_buf[data_size-3]&0x0f)*100 + (temp_buf[data_size-2]>>4)*10 + (temp_buf[data_size-2]&0x0f) + (temp_buf[data_size-1]>>4)*0.1 + (temp_buf[data_size-1]&0x0f)*0.01);
//						//printf("Sum ok!\n");
//					 }
//					 
//					RECIVE_start = 0;
//					rec_count = 0;	
//					data_size = 0;
//					
//			
//				}
//				
//			}
//				

//		} 
//	}
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
