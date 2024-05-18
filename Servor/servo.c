#include "main.h"
#include "tim.h"

void ServoControl(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t Angle)
{
	__HAL_TIM_SetCompare(htim, Channel, Angle);
}