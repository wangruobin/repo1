#include "stm32h7xx_hal_tim.h"
#include "stm32h7xx_hal.h"

//���̽���ṹ�壺
 struct chassis_decompose_t{
	int16_t vx; //r0
	int16_t vy;	//f0
  int16_t omega;	//y0
 };
 extern struct chassis_decompose_t move;
 extern struct chassis_decompose_t delta_move ;

