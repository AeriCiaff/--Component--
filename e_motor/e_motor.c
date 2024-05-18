#include "main.h"
#include "tim.h"

/*
	* @:无刷电机初始化
	* @:Motor_Init(&htim1, TIM_CHANNEL_1)
	* @:&htimx:选择使用的定时器
	* @:TIM_CHANNEL_1:选择使用的通道
*/
void Motor_Init(TIM_HandleTypeDef *htim, uint32_t Channel)
{
	// 初始化电调
	__HAL_TIM_SetCompare(htim, Channel, 2000);
	HAL_Delay(4000);
	__HAL_TIM_SetCompare(htim, Channel, 1000);
	HAL_Delay(4000);
}



/*
	* @:无刷电机转速控制
	* @:Motor_Speed(&htim1, TIM_CHANNEL_1, 1500)
	* @:&htimx:选择使用的定时器
	* @:TIM_CHANNEL_1:选择使用的通道
	* @:1500:选择占空比(当频率为50Hz时，范围时1ms~2ms,其中1ms为停止，2ms为满速)
*/
void Motor_Speed(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t Speed)
{
	__HAL_TIM_SetCompare(htim, Channel, Speed);
}