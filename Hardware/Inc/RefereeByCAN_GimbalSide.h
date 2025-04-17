/**
 * @file RefereeByCAN_GimbalSide.h
 * @brief RoboMaster 裁判系统通信
 * @attention
 * @hardware
 * @version 
 * @date 2024-03-24
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __RefereeByCAN_GimbalSide_H
#define __RefereeByCAN_GimbalSide_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)
typedef struct {
		uint8_t Robot_ID;
		uint8_t Robot_Level;
		uint16_t Shooter_Barrel_Cooling_Value;
		uint16_t Shooter_Barrel_Heat_Limit;
		uint16_t Chassis_Power_Limit;
} RefByCAN_RobotStatus_t;

typedef struct {
		uint16_t Chassis_Voltage;
		uint16_t Chassis_Current;
		float Chassis_Power;
} RefByCAN_ChassisPower_t;

typedef struct {
		uint16_t Chassis_Power_Buffer;
		uint16_t Shooter_17mm_1_Barrel_Heat;
		uint16_t Shooter_17mm_2_Barrel_Heat;
		uint16_t Shooter_42mm_Barrel_Heat;
} RefByCAN_Heat_t;

#pragma pack()

typedef struct {
		RefByCAN_RobotStatus_t RobotStatus;
		RefByCAN_ChassisPower_t ChassisPower;
		RefByCAN_Heat_t Heat;
} RefByCAN_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __RefereeByCAN_GimbalSide_H */

/*********************************END OF FILE*********************************/
