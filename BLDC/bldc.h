#ifndef __BLDC_H
#define __BLDC_H

#include "tim.h"

/*
	* @:无刷电机(电调)初始化
	* @:Motor_Init(&htim1, TIM_CHANNEL_1)
	* @:&htimx:选择使用的定时器
	* @:TIM_CHANNEL_1:选择使用的通道
*/
void Motor_Init(TIM_HandleTypeDef *htim, uint32_t Channel);


/*
	* @:无刷电机转速控制
	* @:Motor_Speed(&htim1, TIM_CHANNEL_1, 1500)
	* @:&htimx:选择使用的定时器
	* @:TIM_CHANNEL_1:选择使用的通道
	* @:1500:选择占空比(当频率为50Hz时，范围时1ms~2ms,其中1ms为停止，2ms为满速)
*/
void Motor_Speed(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t Speed);

#endif
