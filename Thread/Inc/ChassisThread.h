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
#include "Robodefine.h"

#include "dm_imu.h"

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus

enum ChassisBehaviour_e {
	CHASSIS_ZERO_FORCE = 0,        // 底盘无力
	CHASSIS_NO_MOVE,               // 底盘保持不动
	CHASSIS_NO_FOLLOW,             // 底盘不跟随云台
	CHASSIS_FOLLOW_GIMBAL,         // 底盘跟随云台
	CHASSIS_SPIN,                  // 小陀螺
};

enum Gimbal_Motor_Type{
	Gimbal_BigYawMotor = 0,             //大Yaw
	Gimbal_SmallYawMotor,               //小Yaw
};
enum Control_type{
	Imu = 0,             //IMU
	Machine,             //机械角度控制
};

enum Gimbal_Control_Type{
	Position_Control = 0,          //位控
	Speed_Control,                 //速控
};

class Chassis_t {
	public:

	    //底盘运动状态
		ChassisBehaviour_e 	ChassisBehaviour;
		ChassisBehaviour_e  ChassisBehaviour_Last;
        
		//底盘代码控制的电机
		RM_Motor_t* Chassis_Motor[4];
		RM_Motor_t* Gimbal_Motor[2];

		//电机PID
		PID_t Chasssis_Motor_PID[4];                 //只有四个电机的速度PID
		PID_t Gimbal_Motor_PID[4];                   //分为速度PID和角度PID
       
		//模式PID
		PID_t Chassis_Follow_PID;       
        PID_t Prv_PID_PowerLimit;
		PID_t Gimbal_Zeropoint_Calibration[2];
		PID_t Chassis_Zeropoint_Calibration;

        //电机目标值
		float Motor_Target_Speed[4];                
        float Gimbal_Target_Angle[2];

		//底盘当前速度
		float Chassis_Currentspeed_X;
		float Chassis_Currentspeed_Y;
		float Chassis_Currentspeed_Z;

        //小陀螺转速
		float Prv_Spin_Speed;                       

		//云台于底盘相对角速度速度
		float Yaw_RelativeAngularVelocity;            // 云台相对底盘的Yaw轴电机角速度
		float Pitch_RelativeAngularVelocity;          // 云台相对底盘的Pitch轴电机角速度

		
		bool Prv_Flag_Transit;
		bool Prv_Flag_PowerLimit;
		float Prv_PowerLimit_Target;
		float Prv_TransitionLPFq;  // 取值 0.0~1.0

	
		void  CalcSpeedWithRelativeAngle(void);
		void  FK_ChassisSpeed(void);
		void  IK_MotorSpeed(void);
		void  Calculate_MotorPID(void); 
		void  Gimbal_SelfStabilizing(Control_type Motor_Type);

	public:
        float Yaw_RelativeAngle;                      // 底盘相对云台坐标系的角度 (rad)(-pi,pi)
		uint8_t BigGimbal_ZeroMechanical;                      // 底盘相对云台坐标系的角度 (rad)(-pi,pi)
		float Lnitial_Angle_Deviation;              //云台和底盘同向时的起始角度偏差

        struct {
			float X;
			float Y;
			float Z;
		} Chassis_Target_Speed;            // 底盘坐标系速度目标向量

		struct {
			float X;
			float Y;
			float Z;
		} Gimbal_Target_Speed;             // 云台坐标系目标速度向量

        struct 
		{
			float Yaw_Angle;
			float Pitch_Angle;                //360
			float X_Acceleration;             //rad/s
			float Y_Acceleration;
			float Z_Acceleration;
		} Chassis_Presently_Attitude;         //底盘坐标系相对大地坐标系的姿态信息

		struct 
		{
			float Yaw_Angle;
			float Pitch_Angle;                //360
			float X_Acceleration;             //rad/s
			float Y_Acceleration;
			float Z_Acceleration;
		} Gimbal_Presently_Attitude;          //云台坐标系相对大地坐标系的姿态信息

		Chassis_t() {
			Gimbal_Target_Speed.X = 0;
			Gimbal_Target_Speed.Y = 0;
			Gimbal_Target_Speed.Z = 0;
			Chassis_Target_Speed.X = 0;
			Chassis_Target_Speed.Y = 0;
			Chassis_Target_Speed.Z = 0;
			Chassis_Currentspeed_X = 0;
		    Chassis_Currentspeed_Y = 0;
		    Chassis_Currentspeed_Z = 0;
			Yaw_RelativeAngle = 0;
			BigGimbal_ZeroMechanical = 0;
			Prv_Flag_PowerLimit = 0;
			Prv_PowerLimit_Target = 60.0f;
			Prv_TransitionLPFq = 2.0f * pi * 0.004f * 1.0f;
		}
		void Chassis_Init(RM_Motor_t* Motor_LU, RM_Motor_t* Motor_RU, RM_Motor_t* Motor_LD, RM_Motor_t* Motor_RD);
		void Gimbal_Init(RM_Motor_t* Gimbal_Motor1, RM_Motor_t* Gimbal_Motor2);
		void SetBehaviour(ChassisBehaviour_e Behaviour);
		void UpdataRelativeAttitude_Gyroscope(void);
		void UpdataRelativeAttitude_Mechanical(void);
		void SetPowerLimitFlag(bool doPowerLimit);
		void SetPowerLimitTarget(float Watt);
		void SetChassisTargetSpeed(float Speed_X, float Speed_Y, float Speed_Z);
		void UpdateChassisAttitude(float Yaw_Angle, float Pitch_Angle, float X_Acceleration, float Y_Acceleration, float Z_Acceleration);
		void SetGimbalTargetSpeed(float Speed_Z);
		void SetGimbalTargetAngle(Control_type control_type,Gimbal_Control_Type Control_Type, float Target_Angle);
		void UpdateGimbalAttitude(float Yaw_Angle, float Pitch_Angle, float X_Acceleration, float Y_Acceleration, float Z_Acceleration);
		void GetChassisSpeed(float* Chassis_X, float* Chassis_Y, float* Chassis_Z);
		void Generate(void);
		void GimbalAngle_Calibration_Start(void);
		void GimbalAngle_Calibration_Sport(void);
		void UpadteGimbalAngleError(void);
		
};

Chassis_t* ChassisPoint(void);
#endif

void ChassisThread_Init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ChassisThread_H */

/*********************************END OF FILE*********************************/
