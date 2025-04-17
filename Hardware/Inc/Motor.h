/**
 * @file Motor.h
 * @brief 电机类
 * @attention
 * @hardware 
 * @version 0.1
 * @date 2023-03-03
 * @author tianxu, he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __Motor_H
#define __Motor_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
class Motor_t {
	private:
		uint16_t prv_Angle;         // 转子机械角度 0~8191 (0°~360°)
		int16_t prv_Speed;          // 转子转速 n (RPM)
		int16_t prv_TorqueCurrent;  // 实际转矩电流 I_e ，其中 U_e * I_e = T_e * Ω 
		uint8_t prv_Temp;           // 电机温度
		// int16_t prv_RecvNull;

		uint16_t prv_Angle_Last;

		float prv_Speed_rads;    // 转子转速 Ω (rad/s)
		int32_t prv_Round;       // 转子旋转步路
		int32_t prv_TotalAngle;  // 转子总角度

		int16_t prv_CurrentSetting;        // 电调电流设定值

	public:
		Motor_t() {
			prv_Angle = 0;
			prv_Speed = 0;
			prv_TorqueCurrent = 0;
			prv_Temp = 0;
			// prv_RecvNull = 0;

			prv_Angle_Last = 0;

			prv_Speed_rads = 0.0f;
			prv_Round = 0;
			prv_TotalAngle = 0;

			prv_CurrentSetting = 0;
		}
		int16_t GetTorqueCurrent(void);
		int16_t GetSpeed(void);
		uint16_t GetAngle(void);
		uint8_t GetTemp(void);

		float GetSpeedRads(void);
		int32_t GetRound(void);
		void ClearRound(void);

		void SetCurrent(int16_t Current);
};

#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __Motor_H */

/*********************************END OF FILE*********************************/
