#include "main.h"
#include "tim.h"


void Motor_Init(TIM_HandleTypeDef *htim, uint32_t Channel)
{
	__HAL_TIM_SetCompare(htim, Channel, 2000);
	HAL_Delay(4000);
	__HAL_TIM_SetCompare(htim, Channel, 1000);
	HAL_Delay(4000);
}

void Motor_Speed(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t Speed)
{
	__HAL_TIM_SetCompare(htim, Channel, Speed);
}