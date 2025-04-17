/**
 * @file IMU_CAN.h
 * @brief 维特智能陀螺仪（经过串口转CAN）
 * @attention
 * @hardware 
 * @version 0.2
 * @date 2024-03-22
 * @author tianxu, he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __IMU_CAN_H
#define __IMU_CAN_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	float Angle_X;  // 欧拉角
	float Angle_Y;
	float Angle_Z;

	float Palst_X;  // 角速度
	float Palst_Y;
	float Palst_Z;

	float Accel_X;  // 加速度
	float Accel_Y;
	float Accel_Z;

	float Magne_X;  // 磁场
	float Magne_Y;
	float Magne_Z;

	float Heigh_P;  // 气压
	float Heigh_H;  // 高度

	float Temp;  // 温度
} IMU_CAN_t;

IMU_CAN_t* IMU_CAN_Point(uint8_t Code);

void IMU_CAN_Init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __IMU_CAN_H */

/*********************************END OF FILE*********************************/
