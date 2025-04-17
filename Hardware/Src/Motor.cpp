/**
 * @file Motor.cpp
 * @brief 电机类
 * @attention 
 * @hardware 
 * @version 0.2
 * @date 2023-03-03
 * @author tianxu, he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "Motor.h"

#include "Basic.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"

/**
 * @brief 获取电机转动步路
 */
int32_t Motor_t::GetRound(void) {
	return prv_Round;
}

/**
 * @brief 电机转动步路清零
 */
void Motor_t::ClearRound(void) {
	prv_Round = 0;
}

/**
 * @brief 获取电机机械角度
 * @return 电机机械角度 0~8191 (0°~360°)
 */
uint16_t Motor_t::GetAngle(void) {
	return prv_Angle;
}

/**
 * @brief 获取电机转子速度
 * @return 电机转子速度(RPM)
 */
int16_t Motor_t::GetSpeed(void) {
	return prv_Speed;
}

/**
 * @brief 获取电机转矩电流
 */
int16_t Motor_t::GetTorqueCurrent(void) {
	return prv_TorqueCurrent;
}

/**
 * @brief 获取电机转子温度
 * @return 电机转子温度(℃)
 */
uint8_t Motor_t::GetTemp(void) {
	return prv_Temp;
}

/**
 * @brief 获取电机转子速度
 * @return 电机转子速度 (rad/s)
 */
float Motor_t::GetSpeedRads(void) {
	return prv_Speed_rads;
}


/**
 * @brief 设置电机的电调输出值
 */
void Motor_t::SetCurrent(int16_t Current) {
	prv_CurrentSetting = Current;
}

/*********************************END OF FILE*********************************/
