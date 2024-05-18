#ifndef __E_MOTOR_H
#define __E_MOTOR_H

#include "tim.h" // 确保包含了定时器的头文件

// 无刷电机初始化函数原型
void Motor_Init(TIM_HandleTypeDef *htim, uint32_t Channel);

// 无刷电机转速控制函数原型
void Motor_Speed(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t Speed);

#endif // __MOTOR_H
