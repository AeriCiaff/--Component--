#ifndef WT1_IMU_H
#define WT1_IMU_H

#include "stdint.h"
/*
    *@: WT_Solve(witdata, Acc, AngleSpeed, Angle)
    *@: witdata: 使用串口接收到的数据
    *@: Acc: 数组，用来存放处理后的Acc数据
    *@: AngleSpeed: 数组，用来存放处理后的AngleSpeed数据
    *@: Angle: 数组，用来存放处理后的Angle数据
*/
int WT_Solve(uint8_t * wit_data,float* Acc,float* AngleSpeed,float* Angle);

#endif