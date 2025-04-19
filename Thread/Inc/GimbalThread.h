/**
 * @file GimbalThread.h
 * @brief 云台线程
 * @attention 依赖项 Motor, IMU, PID
 * @hardware H-V1 H-V2
 * @version 0.1
 * @date 2024-03-13
 * @author tianxu he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __GimbalThread_H
#define __GimbalThread_H

#include "main.h"

#include "math.h"
#include "LPF.h"
#include "PID.h"

#include "BSP_IMU.h"

#include "IMU_CAN.h"
#include "RM_Motor.h"

#define PI 3.1415926535897932384626f

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
enum GimbalBehaviour_e {
	GIMBAL_ZERO_FORCE,  // 云台无力
	GIMBAL_ENABLE,      // 云台正常工作
	GIMBAL_NO_MOVE,     // 云台不动
	GIMBAL_FOLLOW,      // 云台辅助瞄准
};
class Gimbal_t {
	private:
		GimbalBehaviour_e Private_Behaviour;

		RM_Motor_t* Private_Motor_Yaw;
		RM_Motor_t* Private_Motor_Yaw2;
		RM_Motor_t* Private_Motor_Pitch;
		RM_Motor_t* Private_Motor_Pitch2;

		uint32_t Private_Zero_Yaw;
		uint32_t Private_Zero_Yaw2;
		uint32_t Private_Zero_Pitch;
		uint32_t Private_Zero_Pitch2;

		float Private_Angle_Yaw_Target;
		float Private_Angle_Pitch_Target;

		PID_t Private_PID_IMU_Yaw_Speed;
		PID_t Private_PID_IMU_Yaw_Angle;

		PID_t Private_PID_IMU_Pitch_Speed;
		PID_t Private_PID_IMU_Pitch_Angle;

		float Private_Speed_Yaw_Target;

		LPF_t Private_LPF;

		IMU_CAN_t* Private_IMU;

	public:
		Gimbal_t() {
			Private_IMU = NULL;
		}
		void Init(RM_Motor_t* Motor_Yaw, uint32_t Zero_Yaw, RM_Motor_t* Motor_Pitch, uint32_t Zero_Pitch, IMU_t* IMU_OnGimbal);
		void Init(RM_Motor_t* Motor_Yaw, uint32_t Zero_Yaw, RM_Motor_t* Motor_Pitch, uint32_t Zero_Pitch, IMU_CAN_t* IMU_OnGimbal);
		void Init(RM_Motor_t* Motor_Yaw, uint32_t Zero_Yaw, RM_Motor_t* Motor_Pitch, uint32_t Zero_Pitch, RM_Motor_t* Motor_Pitch2, uint32_t Zero_Pitch2, IMU_CAN_t* IMU_OnGimbal);
		void SetBehaviour(GimbalBehaviour_e Behaviour);
		void SetAngle(float Yaw, float Pitch);
		void SetSpeed(float Yaw, float Pitch);
		void SetAutoFollow(float Speed_X, float Angle_Y);
		float GetRelativeAngle(void);
		void Generate(void);
};
Gimbal_t* GimbalPoint(uint8_t Code);
#endif

void GimbalThread_Init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __GimbalThread_H */

/*********************************END OF FILE*********************************/
