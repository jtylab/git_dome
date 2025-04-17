/**
 * @file IMU_CAN.cpp
 * @brief 维特智能陀螺仪（经过串口转CAN）
 * @attention 依赖 BSP_CAN
 * @hardware
 * @version 0.2
 * @date 2024-03-22
 * @author tianxu, he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "IMU_CAN.h"

#include "BSP_CAN.h"

#define IMU_NUM 3

IMU_CAN_t IMU[IMU_NUM];

void IMU_CAN_Parser(IMU_CAN_t *IMU, uint32_t ID, uint8_t *Data);
void IMU_CAN_RxCbHandler(uint8_t Port, uint32_t ID, uint8_t *Data);

/**
 * @brief CAN总线陀螺仪初始化
 */
void IMU_CAN_Init(void) {
	BSP_CAN_SetRxAllCallbackFunc(IMU_CAN_RxCbHandler);
	// BSP_CAN_SetRxCallbackFunc(1, [](uint32_t ID, uint8_t* Data){IMU_CAN_Parser(&IMU[0], ID, Data);});
	// BSP_CAN_SetRxCallbackFunc(2, [](uint32_t ID, uint8_t* Data){IMU_CAN_Parser(&IMU[1], ID, Data);});
}

/**
 * @brief 陀螺仪 CAN 接收数据处理
 * @param IMU 指针
 * @param ID CAN ID
 * @param Data 接收数据指针
 * @note 非用户调用函数
 */
void IMU_CAN_Parser(IMU_CAN_t *IMU, uint32_t ID, uint8_t *Data) {
	switch (ID) {  // clang-format off
		case 0x100:  // 欧拉角
			IMU->Angle_X = (*((int16_t *)(&Data[0]))) / 32768.0f * 180.0f;  // 单位:°
			IMU->Angle_Y = (*((int16_t *)(&Data[2]))) / 32768.0f * 180.0f;  // 单位:°
			IMU->Angle_Z = (*((int16_t *)(&Data[4]))) / 32768.0f * 180.0f;  // 单位:°
			break;
		case 0x101:  // 角速度
			IMU->Palst_X = (*((int16_t *)(&Data[0]))) / 32768.0f * 2000.0f;  // 单位:°/s
			IMU->Palst_Y = (*((int16_t *)(&Data[2]))) / 32768.0f * 2000.0f;  // 单位:°/s
			IMU->Palst_Z = (*((int16_t *)(&Data[4]))) / 32768.0f * 2000.0f;  // 单位:°/s
			break;
		case 0x102:  // 加速度
			IMU->Accel_X = (*((int16_t *)(&Data[0]))) / 32768.0f * 16.0f * 9.8f;  // 单位:m/(s^2)
			IMU->Accel_Y = (*((int16_t *)(&Data[2]))) / 32768.0f * 16.0f * 9.8f;  // 单位:m/(s^2)
			IMU->Accel_Z = (*((int16_t *)(&Data[4]))) / 32768.0f * 16.0f * 9.8f;  // 单位:m/(s^2)
			IMU->Temp    = (*((int16_t *)(&Data[6]))) / 100.0f;                   // 单位:℃
			break;
		case 0x103:  // 磁场
			IMU->Magne_X = (*((int16_t *)(&Data[0])));
			IMU->Magne_Y = (*((int16_t *)(&Data[2])));
			IMU->Magne_Z = (*((int16_t *)(&Data[4])));
			break;
		case 0x104:
			IMU->Heigh_P = (*((int32_t *)(&Data[0])));  // 气压 单位:Pa
			IMU->Heigh_H = (*((int32_t *)(&Data[4])));  // 高度 单位:cm
			break;
	}  // clang-format on
}

/**
 * @brief 注册给 BSP_CAN 回调的函数
 * @note 非用户调用函数
 */
void IMU_CAN_RxCbHandler(uint8_t Port, uint32_t ID, uint8_t *Data) {
	if (Port > 0 && Port <= IMU_NUM) {
		IMU_CAN_Parser(&IMU[Port - 1], ID, Data);
	}
}

/**
 * @brief 陀螺仪数据指针
 * @param CAN 总线号
 * @return class IMU_CAN_t*
 */
IMU_CAN_t *IMU_CAN_Point(uint8_t Code) {
	if (Code > 0 && Code <= IMU_NUM) {
		return &IMU[Code - 1];
	}
	return NULL;
}

/*********************************END OF FILE*********************************/
