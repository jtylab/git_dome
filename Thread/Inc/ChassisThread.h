/**
 * @file ChassisThread.h
 * @brief 底盘线程
 * @attention
 * @hardware
 * @version 0.2
 * @date 2024-03-17
 * @author tianxu, he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __ChassisThread_H
#define __ChassisThread_H

#include "main.h"

#include "PID.h"
// #include "LPF.h"

#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "math.h"


#include "RM_Motor.h"


#include "BSP_KEY.h"
#include "BSP_PWM.h"
#include "BSP_UART.h"
#include "BSP_IMU.h"

#include "Basic.h"
#include "Kalman.h"
#include "LPF.h"
#include "PID.h"

#include "DR16.h"
#include "IMU_CAN.h"
#include "RM_Motor.h"
#include "RM_VTM.h"
#include "Referee.h"
#include "SuperCap.h"
#include "VOFA.h"

#include "ChassisThread.h"
#include "DebuggerThread.h"
#include "MainThread.h"



#ifdef __cplusplus
extern "C" {
#endif

#define LU 0
#define RU 1
#define LD 2
#define RD 3

#define PI 3.1415926535897932384626f

#define LU_Angle    1.0f/4.0f*PI
#define RU_Angle   -1.0f/4.0f*PI
#define LD_Angle    3.0f/4.0f*PI
#define RD_Angle   -3.0f/4.0f*PI
#define Chassis_R   0.25f                        //???????????????????  底盘中心到电机的半径(需要测量)
#define Motort_R    0.075f                        //???????????????????  电机中心到轮子的半径(需要测量)

#ifdef __cplusplus

enum ChassisBehaviour_e {
	CHASSIS_ZERO_FORCE = 0, // 底盘无力
	CHASSIS_NO_MOVE,        // 底盘保持不动
	CHASSIS_NO_FOLLOW,      // 底盘不跟随云台
	CHASSIS_FOLLOW_GIMBAL,  // 底盘跟随云台
	CHASSIS_SPIN,           // 小陀螺
};
class Chassis_t {
	private:
		ChassisBehaviour_e Prv_Behaviour;
		ChassisBehaviour_e Prv_Behaviour_Last;

		RM_Motor_t* Prv_Motor[4];
		PID_t Prv_PID_Motor_Speed[4];
		float Prv_Motor_Speed_Target[4];
		float Chassis_Currentspeed_X;
		float Chassis_Currentspeed_Y;
		float Chassis_Currentspeed_Z;

		struct {
			float X;
			float Y;
			float Z;
		} Prv_Speed;  // 底盘坐标系速度向量

		struct {
			float X;
			float Y;
			float Z;
		} Prv_TargetSpeed;  // 目标速度向量

		float Prv_Spin_Speed;  // 小陀螺转速
		float Prv_MaxSpinSpeed;

		float Prv_RelativeAngle;  // 底盘相对云台坐标系的角度 (rad)(0~2Pi)

		PID_t Prv_PID_Follow;

		bool Prv_Flag_Transit;

		bool Prv_Flag_PowerLimit;
		float Prv_PowerLimit_Target;
		PID_t Prv_PID_PowerLimit;

		float Prv_TransitionLPFq;  // 取值 0.0~1.0

	private:
		float SpinSpeedGenerate(void);
		void  CalcSpeedWithRelativeAngle(void);
		void  FK_ChassisSpeed(void);
		void  IK_MotorSpeed(void);

	public:
		Chassis_t() {
			Prv_TargetSpeed.X = 0;
			Prv_TargetSpeed.Y = 0;
			Prv_TargetSpeed.Z = 0;
			Prv_Speed.X = 0;
			Prv_Speed.Y = 0;
			Prv_Speed.Z = 0;
			Chassis_Currentspeed_X = 0;
		    Chassis_Currentspeed_Y = 0;
		    Chassis_Currentspeed_Z = 0;
			Prv_Spin_Speed = 0;
			Prv_RelativeAngle = 0;
			Prv_Flag_PowerLimit = 0;
			Prv_PowerLimit_Target = 60.0f;
			Prv_MaxSpinSpeed = 6000.0f;
			Prv_TransitionLPFq = 2.0f * pi * 0.004f * 1.0f;
		}
		void Init(RM_Motor_t* Motor_LU, RM_Motor_t* Motor_RU, RM_Motor_t* Motor_LD, RM_Motor_t* Motor_RD);
		void SetBehaviour(ChassisBehaviour_e Behaviour);
		void SetTrendAngle(float Trend_Angle);
		void SetPowerLimitFlag(bool doPowerLimit);
		void SetPowerLimitTarget(float Watt);
		void SetChassisSpeed(float Speed_X, float Speed_Y, float Speed_Z);
		void GetChassisSpeed(float* Chassis_X, float* Chassis_Y, float* Chassis_Z);
		void Generate(void);
};

Chassis_t * ChassisPoint(void);

#endif

void ChassisThread_Init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ChassisThread_H */

/*********************************END OF FILE*********************************/
