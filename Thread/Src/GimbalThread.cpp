/**
 * @file GimbalThread.cpp
 * @brief 云台线程
 * @attention 依赖项 Motor, IMU, PID
 * @hardware H-V1 H-V2
 * @version 0.2
 * @date 2024-03-13
 * @author tianxu he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "GimbalThread.h"


#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"


#define GIMBAL_NUM 1  // 云台的数量

#define Pitch_Angle_Upper_Limit 20
#define Pitch_Angle_Lower_Limit -30

static Gimbal_t Gimbal[GIMBAL_NUM];

/**
 * @brief 云台初始化函数
 * @param Motor_Yaw Yaw轴电机
 * @param Zero_Yaw Yaw轴电机零点
 * @param Motor_Pitch Pitch轴电机
 * @param Zero_Pitch Pitch轴电机零点
 * @param 陀螺仪（不使用可为NULL）
 */
void Gimbal_t::Init(RM_Motor_t* Motor_Yaw, uint32_t Zero_Yaw,
					RM_Motor_t* Motor_Pitch, uint32_t Zero_Pitch,
					IMU_CAN_t* IMU_OnGimbal) {  // clang-format off
	Private_Motor_Yaw   = Motor_Yaw;
	Private_Motor_Pitch = Motor_Pitch;
	Private_Zero_Yaw    = Zero_Yaw;
	Private_Zero_Pitch  = Zero_Pitch;
	Private_IMU         = IMU_OnGimbal;
	// clang-format on

	// Private_LPF_Motor_IMU_Speed.Init(4, 5);
	// Private_LPF_Motor_Current.Init(4, 5);

	Private_PID_IMU_Pitch_Angle.Init(5, 0, 0.1,0, 4, 100, -1);
	Private_PID_IMU_Pitch_Speed.Init(62, 1.0, 65,0, 4, 15000, 25000, 200.0, 0.5);

	Private_PID_IMU_Yaw_Angle.Init(7, 0, 10,0,4, 100, -1);
	Private_PID_IMU_Yaw_Speed.Init(95, 0, 10, 0,4, 15000, 25000, 200.0, 0.5);
}

/**
 * @brief 云台初始化函数
 * @param Motor_Yaw Yaw轴电机
 * @param Zero_Yaw Yaw轴电机零点
 * @param Motor_Pitch Pitch轴电机
 * @param Zero_Pitch Pitch轴电机零点
 * @param Motor_Pitch2 Pitch轴电机2
 * @param Zero_Pitch2 Pitch轴电机2零点
 * @param 陀螺仪（不使用可为NULL）
 */
void Gimbal_t::Init(RM_Motor_t* Motor_Yaw, uint32_t Zero_Yaw,
					RM_Motor_t* Motor_Pitch, uint32_t Zero_Pitch,
					RM_Motor_t* Motor_Pitch2, uint32_t Zero_Pitch2,
					IMU_CAN_t* IMU_OnGimbal) {  // clang-format off
	Private_Motor_Yaw    = Motor_Yaw;
	// Private_Motor_Yaw2   = Motor_Yaw2;
	Private_Motor_Pitch  = Motor_Pitch;
	Private_Motor_Pitch2 = Motor_Pitch2;
	Private_Zero_Yaw     = Zero_Yaw;
	// Private_Zero_Yaw2    = Zero_Yaw2;
	Private_Zero_Pitch   = Zero_Pitch;
	Private_Zero_Pitch2  = Zero_Pitch2;
	Private_IMU          = IMU_OnGimbal;
	// clang-format on

	Private_Angle_Yaw_Target = Private_IMU->Angle_Z;  // +180.0
	Private_Speed_Yaw_Target = 0;
	Private_LPF.Init(5, 100);

	Private_PID_IMU_Pitch_Angle.Init(13, 0.001, 0.1,0, 4, 1000, -1);
	Private_PID_IMU_Pitch_Speed.Init(100, 1.0, 600,0, 4, 15000, 30000, 600.0f, 0.5f);

	Private_PID_IMU_Yaw_Angle.Init(9.0, 0.001, 1000,0, 4, 1000, -1);  // 8.51
	Private_PID_IMU_Yaw_Speed.Init(150, 0.1, 2000,0, 4, 10000, 30000, 600.0f, 0.5f);
}

/**
 * @brief 云台设置角度函数
 * @param Yaw Yaw轴电机角度
 * @param Pitch Pitch轴电机角度
 */
void Gimbal_t::SetAngle(float Yaw, float Pitch) {
	Private_Angle_Yaw_Target -= Yaw;
	Private_Angle_Pitch_Target = Pitch;
}

/**
 * @brief 云台设置旋转速度函数
 * @param Yaw Yaw轴电机速度
 * @param Pitch Pitch轴电机速度
 */
void Gimbal_t::SetSpeed(float Yaw, float Pitch) {
	Private_Angle_Yaw_Target -= Yaw;
	Private_Angle_Pitch_Target += Pitch;
}

/**
 * @brief 云台设置行为函数
 */
void Gimbal_t::SetBehaviour(GimbalBehaviour_e Behaviour) {
	Private_Behaviour = Behaviour;
}

/**
 * @brief 云台设置自动跟随
 * @param Speed_X Yaw轴电机速度
 * @param Angle_Y Pitch轴电机角度
 */
void Gimbal_t::SetAutoFollow(float Speed_X, float Angle_Y) {
	Private_Speed_Yaw_Target = Speed_X;
	Private_Angle_Pitch_Target = Angle_Y;
}

/**
 * @brief 取云台相对角度
 * @return float 角度
 */
float Gimbal_t::GetRelativeAngle(void) {
	float Output = 0.0f;
	Output = Private_Motor_Yaw->GetAngle() * 1.0f - Private_Zero_Yaw * 1.0f;
	Output = (Output * 360.0f) / 8192.0f;
	while (Output > 360.0f) {
		Output -= 360.0f;
	}
	while (Output < 0.0f) {
		Output += 360.0f;
	}
	return Output;
}

/**
 * @brief 云台计算
 */
void Gimbal_t::Generate(void) {
	float PitchAngleError = ((float)Private_Zero_Pitch - (float)Private_Motor_Pitch->GetAngle()) * 360.0f / 8192.0f - Private_IMU->Angle_X;

	if (PitchAngleError > 180.0f)  PitchAngleError -= 360.0f;
	else if (PitchAngleError < -180.0f) PitchAngleError += 360.0f;

	if (Private_IMU->Angle_X == 0.0f) {
		Private_Angle_Pitch_Target = LimitBoth(Private_Angle_Pitch_Target, Pitch_Angle_Upper_Limit, Pitch_Angle_Lower_Limit);
	} else {
		Private_Angle_Pitch_Target = LimitBoth(Private_Angle_Pitch_Target, Pitch_Angle_Upper_Limit, Pitch_Angle_Lower_Limit);  // 有bug，临时禁用
		// Private_Angle_Pitch_Target = LimitBoth(Private_Angle_Pitch_Target, Pitch_Angle_Upper_Limit - PitchAngleError, Pitch_Angle_Lower_Limit - PitchAngleError);
	}

	switch (Private_Behaviour) {
		case GIMBAL_ZERO_FORCE:  // 云台无力
			Private_PID_IMU_Pitch_Angle.ClearIntegral();
			Private_PID_IMU_Pitch_Speed.ClearIntegral();
			Private_PID_IMU_Yaw_Angle.ClearIntegral();
			Private_PID_IMU_Yaw_Speed.ClearIntegral();
			Private_Motor_Pitch->SetCurrent(0);
			Private_Motor_Pitch2->SetCurrent(0);
			Private_Motor_Yaw->SetCurrent(0);
			Private_Angle_Yaw_Target = Private_IMU->Angle_Z;  // +180.0
			return;                                           // 无力的关键
		case GIMBAL_ENABLE:                                   // 云台正常工作
			// Pitch
			Private_PID_IMU_Pitch_Angle.GenerateRing(Private_IMU->Angle_X, Private_Angle_Pitch_Target, 360.0f);
			Private_PID_IMU_Pitch_Speed.Generate(Private_IMU->Palst_X, Private_PID_IMU_Pitch_Angle.GetOutput());
			Private_Motor_Pitch->SetCurrent(Private_PID_IMU_Pitch_Speed.GetOutput());
			Private_Motor_Pitch2->SetCurrent(Private_PID_IMU_Pitch_Speed.GetOutput());

			// Yaw
			while (Private_Angle_Yaw_Target > 360.0f) Private_Angle_Yaw_Target -= 360.0f;
			while (Private_Angle_Yaw_Target < 0.0f) Private_Angle_Yaw_Target += 360.0f;
			Private_PID_IMU_Yaw_Angle.GenerateRing(Private_IMU->Angle_Z, Private_Angle_Yaw_Target, 360.0);  // +180.0
			Private_PID_IMU_Yaw_Speed.Generate(Private_IMU->Palst_Z, Private_PID_IMU_Yaw_Angle.GetOutput());
			Private_Motor_Yaw->SetCurrent(Private_PID_IMU_Yaw_Speed.GetOutput());
			break;
		case GIMBAL_NO_MOVE:

			break;
		case GIMBAL_FOLLOW:  // 云台辅助瞄准
			// Pitch
			Private_PID_IMU_Pitch_Angle.GenerateRing(Private_IMU->Angle_X, Private_Angle_Pitch_Target, 360.0f);
			Private_PID_IMU_Pitch_Speed.Generate(Private_IMU->Palst_X, Private_PID_IMU_Pitch_Angle.GetOutput());
			Private_Motor_Pitch->SetCurrent(Private_PID_IMU_Pitch_Speed.GetOutput());
			Private_Motor_Pitch2->SetCurrent(Private_PID_IMU_Pitch_Speed.GetOutput());

			// Yaw
			Private_PID_IMU_Yaw_Speed.Generate(Private_IMU->Palst_Z, Private_Speed_Yaw_Target);
			Private_Motor_Yaw->SetCurrent(Private_PID_IMU_Yaw_Speed.GetOutput());
			Private_Angle_Yaw_Target = Private_IMU->Angle_Z;  // +180.0
			break;
		default:
			Private_Motor_Pitch->SetCurrent(0);
			Private_Motor_Pitch2->SetCurrent(0);
			Private_Motor_Yaw->SetCurrent(0);
			Private_Angle_Yaw_Target = Private_IMU->Angle_Z;  // +180.0
			return;                                           // 无力的关键
	}
}

/**
 * @brief 取云台指针
 * @return class Gimbal_t*
 */
Gimbal_t* GimbalPoint(uint8_t Code) {
	return &Gimbal[Code];
}

/**
 * @brief 云台线程
 * @call freertos.c
 */
void GimbalTask(void* argument) {
	osDelay(3500);
	while (1) {
		for (int i = 0; i < GIMBAL_NUM; i++) {
			Gimbal[i].Generate();
		}
		osDelay(4);
	}
}

// osThreadId_t GimbalThreadHandle;
// static const osThreadAttr_t GimbalTask_attributes = {"GimbalTask", 0, 0, 0, 0, 512, (osPriority_t)osPriorityNormal3};
// /**
//  * @brief 线程初始化
//  * @call main.c
//  */
// void GimbalThread_Init(void) {
// 	GimbalThreadHandle = osThreadNew(GimbalTask, NULL, &GimbalTask_attributes);  // 创建线程
// }

/*********************************END OF FILE*********************************/
