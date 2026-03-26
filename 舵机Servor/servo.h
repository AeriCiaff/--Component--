#ifndef __SERVO_H
#define __SERVO_H

#include "tim.h"

/*
	*@: 舵机角度控制
	*@: ServoControl(&htim1, TIM_CHANNEL_1, 1500)
	*@: &htim1: 舵机使用的定时器
	*@: TIM_CHANNEL_1: 舵机使用的通道
	*@: 1500: 设定转的角度(此为20ms的周期)
*/
void ServoControl(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t Angle);

#endif