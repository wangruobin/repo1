#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_tim.h"
#include "stm32h7xx_hal_fdcan.h"
#include "stm32h7xx_it.h"
#include "usart.h"
#include "fdcan.h"
#include "main.h"
#include "transmit.h"
#include "wrb_communication.h"
#include "stdlib.h"
#include "usart.h"

//�ٶȷֽ����������
//		vy----->r0  
//		vx----->f0  
//		omega-->y0
struct chassis_decompose_t move =
{
	0,0,0
};
struct chassis_decompose_t delta_move =
{
  0,0,0
};



	uint16_t temp,rx_len;
void USART6_IDLE_CALLBACK()
{

	//HAL_UART_DMAStop(&huart3);
	if(( __HAL_UART_GET_FLAG(&huart6,UART_FLAG_IDLE)!= RESET))//idle��־����λ
	{ 
		Check.Referee=0;    
	  Debug.Referee++;
    __HAL_DMA_DISABLE(&hdma_usart6_rx);		
		__HAL_UART_CLEAR_IDLEFLAG(&huart6);//�����־λ
		//(void)USART3->
		__HAL_UART_CLEAR_OREFLAG(&huart6);
//    (void)huart3.Instance->CR1;
//		temp=huart3.Instance->RDR;
//		temp=huart3.Instance->ISR;		
		temp = __HAL_DMA_GET_COUNTER(&hdma_usart6_rx);// ��ȡDMA��δ��������ݸ���
		rx_len = 128 - temp; //�ܼ�����ȥδ��������ݸ������õ��Ѿ����յ����ݸ���
		//Referee_Data_Diapcak(referee_meta_data,rx_len);
		Referee_Data_Diapcak(User_meta_data,rx_len);
	 __HAL_DMA_SET_COUNTER(&hdma_usart6_rx,128);
//				__HAL_DMA_CLEAR_FLAG(&hdma_usart3_rx,DMA_FLAG_TCIF0_4|DMA_FLAG_HTIF0_4);
//		DMA1_Stream4->NDTR = (uint32_t)128;
	  __HAL_DMA_ENABLE(&hdma_usart6_rx);
		HAL_UART_Receive_DMA(&huart6,User_meta_data,128);//���´�DMA����  
	}		
}