/**
 * @file ChassisThread.cpp
 * @brief 底盘线程
 * @attention
 * @hardware
 * @version 0.2
 * @date 2024-03-17
 * @author tianxu, he, jiang
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "ChassisThread.h"


Chassis_t Chassis;

/**
 * @brief 底盘初始化函数
 * @param LU 左上角电机
 * @param RU 右上角电机
 * @param LD 左下角电机
 * @param RD 右下角电机
 */
void Chassis_t::Init(RM_Motor_t* Motor_LU, RM_Motor_t* Motor_RU, RM_Motor_t* Motor_LD, RM_Motor_t* Motor_RD) {
	Prv_Motor[LU] = Motor_LU;
	Prv_Motor[RU] = Motor_RU;
	Prv_Motor[LD] = Motor_LD;
	Prv_Motor[RD] = Motor_RD;

	Prv_MaxSpinSpeed = 3000.0f;

	// Prv_PID_Motor_Speed[0].Init(13, 0 ,0, 4, 6000, 16383);
	// Prv_PID_Motor_Speed[1].Init(13, 0, 0, 4, 6000, 16383);
	// Prv_PID_Motor_Speed[2].Init(13, 0, 0, 4, 6000, 16383);
	// Prv_PID_Motor_Speed[3].Init(13, 0, 0, 4, 6000, 16383);

	Prv_PID_Motor_Speed[0].Init(1.52000600800904,2.68508859420655,0.0101460207730336,0,4,6000, 16383);
	Prv_PID_Motor_Speed[1].Init(1.52000600800904,2.68508859420655,0.0101460207730336,0,4,6000, 16383);
	Prv_PID_Motor_Speed[2].Init(1.52000600800904,2.68508859420655,0.0101460207730336,0,4,6000, 16383);
	Prv_PID_Motor_Speed[3].Init(1.52000600800904,2.68508859420655,0.0101460207730336,0,4,6000, 16383);


	Prv_PID_Follow.Init(-1000, 0, 0,0, 4 ,3000, 6000);

	Prv_PID_PowerLimit.Init(0.005, 5e-5, 0, 0,4, 0.4, 1);
}

/**
 * @brief 底盘设置行为函数
 */
void Chassis_t::SetBehaviour(ChassisBehaviour_e Behaviour) {
	Prv_Behaviour = Behaviour;
}

/**
 * @brief 置前进方向偏角函数
 * @param Trend_Angle 目标角度，弧度制 (0-2PI)
 */
void Chassis_t::SetTrendAngle(float Trend_Angle) {
	Trend_Angle = 360.0f - Trend_Angle;
	Prv_RelativeAngle = Trend_Angle * pi / 180.0f;
}

/**
 * @brief 底盘设置是否需要功率限制
 */
void Chassis_t::SetPowerLimitFlag(bool doPowerLimit) {
	Prv_Flag_PowerLimit = doPowerLimit;
}

/**
 * @brief 底盘设置是否需要功率限制
 */
void Chassis_t::SetPowerLimitTarget(float Watt) {
	Prv_PowerLimit_Target = Watt;
}

/**
 * @brief Set物理中心的目标速度X,Y,Z(正方向分别为,上,左,逆时针)
 * 
 * @param Speed_X  //物理中心X轴线速度
 * @param Speed_Y  //物理中心Y轴线速度
 * @param Speed_Z  //物理中心旋转角速度
 * 
 * @brief m/s
 */
void Chassis_t::SetChassisSpeed(float Speed_X, float Speed_Y, float Speed_Z) {
	Prv_TargetSpeed.X = Speed_X;
	Prv_TargetSpeed.Y = Speed_Y;
	Prv_TargetSpeed.Z = Speed_Z;
}

/**
 * @brief Get物理中心的当前速度X,Y,Z(正方向分别为,上,左,逆时针)
 * 
 * @param Chassis_X 
 * @param Chassis_Y 
 * @param Chassis_Z 
 * 
 * @brief m/s
 */
void Chassis_t::GetChassisSpeed(float* Chassis_X, float* Chassis_Y, float* Chassis_Z){
	*Chassis_X = Chassis_Currentspeed_X;
	*Chassis_Y = Chassis_Currentspeed_Y;
	*Chassis_Z = Chassis_Currentspeed_Z;
}

/**
 * @brief 运动学逆解算
 * 
 * @brief 从物理中心速度计算每个电机角速度
 * @brief m/s
 */
void Chassis_t::IK_MotorSpeed(void) {
	// clang-format off
	Prv_Motor_Speed_Target[LU] = (cos(LU_Angle)*Prv_Speed.X + sin(LU_Angle)*Prv_Speed.Y + Chassis_R*Prv_Speed.Z) / Motort_R;
	Prv_Motor_Speed_Target[RU] = (cos(RU_Angle)*Prv_Speed.X + sin(RU_Angle)*Prv_Speed.Y + Chassis_R*Prv_Speed.Z) / Motort_R;
	Prv_Motor_Speed_Target[LD] = (cos(LD_Angle)*Prv_Speed.X + sin(LD_Angle)*Prv_Speed.Y + Chassis_R*Prv_Speed.Z) / Motort_R;
	Prv_Motor_Speed_Target[RD] = (cos(RD_Angle)*Prv_Speed.X + sin(RD_Angle)*Prv_Speed.Y + Chassis_R*Prv_Speed.Z) / Motort_R;
	// clang-format on
}

/**
 * @brief 运动学正解算                         
 * 
 * @brief 计算出当前物理中心底盘的运动速度
 * @brief m/s
 */
void Chassis_t::FK_ChassisSpeed(void){
	Chassis_Currentspeed_X = 1.0f / 2.0f * (cos(LU_Angle)*Prv_Motor[LU]->GetSpeed() + cos(RU_Angle)*Prv_Motor[RU]->GetSpeed() + cos(LD_Angle)*Prv_Motor[LD]->GetSpeed() + cos(RD_Angle)*Prv_Motor[RD]->GetSpeed());
	Chassis_Currentspeed_Y = 1.0f / 2.0f * (sin(LU_Angle)*Prv_Motor[LU]->GetSpeed() + sin(RU_Angle)*Prv_Motor[RU]->GetSpeed() + sin(LD_Angle)*Prv_Motor[LD]->GetSpeed() + sin(RD_Angle)*Prv_Motor[RD]->GetSpeed());
	Chassis_Currentspeed_Z = 1.0f / 4.0f * (1.0f / Chassis_R * (Prv_Motor[LU]->GetSpeed() + Prv_Motor[RU]->GetSpeed() + Prv_Motor[LD]->GetSpeed() + Prv_Motor[RD]->GetSpeed()));
}

/**
 * @brief 更新小陀螺转速
 */
float Chassis_t::SpinSpeedGenerate(void) {
	/*
	static float Time = 0;
	Time += 2.0f * PI * 1.0f / 1000.0f;
	if (Time >= 2.0f * PI) {
		Time -= 2.0f * PI;
	}
	// Prv_Spin_Speed = 8900*0.7 + 0.3*8900*((sin(Time)+1)/2.0);
	Prv_Spin_Speed = 4000;  // 8911
	*/

	if (Prv_Behaviour_Last != CHASSIS_SPIN) {
		Prv_Spin_Speed = 1.0f / 4.0f * (Prv_Motor[LU]->GetSpeed() + Prv_Motor[RU]->GetSpeed() + Prv_Motor[LD]->GetSpeed() + Prv_Motor[RD]->GetSpeed());
	}
	if (Prv_Spin_Speed >= Prv_MaxSpinSpeed) {
		Prv_Spin_Speed = Prv_MaxSpinSpeed;
	} else {
		Prv_Spin_Speed += 20;
	}

	return Prv_Spin_Speed;
}

/**
 * @brief 与云台有相对角度时，计算速度向量（应用旋转矩阵，从云台坐标系变换到底盘坐标系）
 */
void Chassis_t::CalcSpeedWithRelativeAngle(void) {
	// clang-format off
	Prv_Speed.X = Prv_TargetSpeed.X *  cos(Prv_RelativeAngle) + Prv_TargetSpeed.Y * sin(Prv_RelativeAngle);
	Prv_Speed.Y = Prv_TargetSpeed.X * -sin(Prv_RelativeAngle) + Prv_TargetSpeed.Y * cos(Prv_RelativeAngle);
	// clang-format on
}



/**
 * @brief 底盘计算
 * @caller ChassisTask
 */
void Chassis_t::Generate(void) {
	float MotorSpeed_Max, SpeedLimitK = 1.0f, K_LimitByPwr = 1.0f;

	switch (Prv_Behaviour) {
		case CHASSIS_ZERO_FORCE:  // 底盘无力模式
			Prv_Motor[LU]->SetCurrent(0);
			Prv_Motor[RU]->SetCurrent(0);
			Prv_Motor[LD]->SetCurrent(0);
			Prv_Motor[RD]->SetCurrent(0);
			Prv_Behaviour_Last = CHASSIS_ZERO_FORCE;
			return;            // 无力的关键
		case CHASSIS_NO_MOVE:  // 底盘不动模式
			Prv_Motor_Speed_Target[LU] = 0;
			Prv_Motor_Speed_Target[RU] = 0;
			Prv_Motor_Speed_Target[LD] = 0;
			Prv_Motor_Speed_Target[RD] = 0;
			Prv_Behaviour_Last = CHASSIS_NO_MOVE;
			break;
		case CHASSIS_NO_FOLLOW:  // 底盘不跟随云台
			Prv_Speed.X = Prv_TargetSpeed.X;
			Prv_Speed.Y = Prv_TargetSpeed.Y;
			Prv_Speed.Z = Prv_TargetSpeed.Z;
			IK_MotorSpeed();
			Prv_Behaviour_Last = CHASSIS_NO_FOLLOW;
			break;
		case CHASSIS_FOLLOW_GIMBAL:  // 底盘跟随云台
			if (Prv_Behaviour_Last != CHASSIS_FOLLOW_GIMBAL) {
				Prv_Flag_Transit = 1;  // 缓启动
				Prv_PID_Follow.SetPID(-5000, 0, 0);
				Prv_PID_Follow.SetLimit(4000, 2000);
			}
			CalcSpeedWithRelativeAngle();
			if (Prv_Flag_Transit && fabs(Prv_RelativeAngle) < 0.05f * 2.0f * PI) {  // 缓启动完成
				Prv_PID_Follow.SetPID(-2000, 0, -500);
				Prv_PID_Follow.SetLimit(3000, 6000);
				Prv_Flag_Transit = 0;
			}
			Prv_Speed.Z = -Prv_PID_Follow.GenerateRing(Prv_RelativeAngle, 0, 2.0f * PI);  // 跟随
			IK_MotorSpeed();
			Prv_Behaviour_Last = CHASSIS_FOLLOW_GIMBAL;
			break;
		case CHASSIS_SPIN:  // 小陀螺运动
			CalcSpeedWithRelativeAngle();
			Prv_Speed.Z = -SpinSpeedGenerate();  // 按该函数更新转速
			IK_MotorSpeed();
			Prv_Behaviour_Last = CHASSIS_SPIN;
			break;
	}

	// 限速，保持各轮子之间的速度比
	MotorSpeed_Max = 0;
	for (int i = 0; i < 4; i++) {
		if (fabs(Prv_Motor_Speed_Target[i]) > MotorSpeed_Max) {
			MotorSpeed_Max = fabs(Prv_Motor_Speed_Target[i]);
		}
	}
	if (MotorSpeed_Max > 8191) {
		SpeedLimitK = 8191.0f / MotorSpeed_Max;
	} else {
		SpeedLimitK = 1.0f;
	}

	// // 功率限制
	// if (Prv_Flag_PowerLimit) {
	// 	Prv_PID_PowerLimit.Generate(Chassis_Power, Prv_PowerLimit_Target);
	// 	K_LimitByPwr = LimitBoth(1 + Prv_PID_PowerLimit.GetOutput(), 1.0, 0.1);
	// 	// K_LimitByPwr = 1.0f;
	// } else {
	// 	K_LimitByPwr = 1.0f;
	// }

	// 计算速度环 PID
	Prv_PID_Motor_Speed[LU].Generate(Prv_Motor[LU]->GetSpeed(), K_LimitByPwr * SpeedLimitK * Prv_Motor_Speed_Target[LU]);
	Prv_PID_Motor_Speed[RU].Generate(Prv_Motor[RU]->GetSpeed(), K_LimitByPwr * SpeedLimitK * Prv_Motor_Speed_Target[RU]);
	Prv_PID_Motor_Speed[LD].Generate(Prv_Motor[LD]->GetSpeed(), K_LimitByPwr * SpeedLimitK * Prv_Motor_Speed_Target[LD]);
	Prv_PID_Motor_Speed[RD].Generate(Prv_Motor[RD]->GetSpeed(), K_LimitByPwr * SpeedLimitK * Prv_Motor_Speed_Target[RD]);

	// 将 PID 计算结果给电机
	Prv_Motor[LU]->SetCurrent(Prv_PID_Motor_Speed[LU].GetOutput());  //  / 6.5f
	Prv_Motor[RU]->SetCurrent(Prv_PID_Motor_Speed[RU].GetOutput());  //  / 6.5f
	Prv_Motor[LD]->SetCurrent(Prv_PID_Motor_Speed[LD].GetOutput());  //  / 6.5f
	Prv_Motor[RD]->SetCurrent(Prv_PID_Motor_Speed[RD].GetOutput());  //  / 6.5f
}

/**
 * @brief 底盘线程
 */
void ChassisTask(void* argument) {
	osDelay(3500);

	while (1) {
		Chassis.Generate();
		osDelay(4);
	}
}

osThreadId_t ChassisThreadHandle;
static const osThreadAttr_t ChassisTask_attributes = {"ChassisTask", 0, 0, 0, 0, 512, (osPriority_t)osPriorityNormal3};
/**
 * @brief 线程初始化
 * @caller main.c
 */
void ChassisThread_Init(void) {
	ChassisThreadHandle = osThreadNew(ChassisTask, NULL, &ChassisTask_attributes);  // 创建底盘线程
}

/**
 * @brief 取底盘类指针
 * @return class Chassis_t*
 */
Chassis_t* ChassisPoint(void) {
	return &Chassis;
}

// /*********************************END OF FILE*********************************/
