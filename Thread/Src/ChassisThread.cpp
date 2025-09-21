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

float Vofabuf[6]={0};


Chassis_t Chassis;
LPF_t LPF_imu , LPF_Gimbal_speed;
uint8_t time_spin = 0;

float RelativeAngle;
float RelativeAngle_two;
/**
 * @brief 底盘电机初始化函数
 * @param LU 左上角电机
 * @param RU 右上角电机
 * @param LD 左下角电机
 * @param RD 右下角电机
 */
void Chassis_t::Chassis_Init(RM_Motor_t* Motor_LU, RM_Motor_t* Motor_RU, RM_Motor_t* Motor_LD, RM_Motor_t* Motor_RD) {
	Chassis_t::Chassis_Motor[LU] = Motor_LU;
	Chassis_t::Chassis_Motor[RU] = Motor_RU;
	Chassis_t::Chassis_Motor[LD] = Motor_LD;
	Chassis_t::Chassis_Motor[RD] = Motor_RD;

	Chassis_t::Chassis_TargetPower = 80.0f;

	Chasssis_Motor_PID[LU].Init(13, 0 ,0, 2, 4, 6000, 16383);
	Chasssis_Motor_PID[RU].Init(13, 0, 0, 2, 4, 6000, 16383);
	Chasssis_Motor_PID[LD].Init(13, 0, 0, 2, 4, 6000, 16383);
	Chasssis_Motor_PID[RD].Init(13, 0, 0, 2, 4, 6000, 16383);

	Chassis_Follow_PID.Init(-1000, 0, 0,0, 4,3000, 16383, 0);

	Prv_PID_PowerLimit.Init(0.005, 5e-5, 0, 0,4, 0.4, 1);

	Chassis_Zeropoint_Calibration.Init(-1, 0, 0,0, 4,3000, 16383, 0);

	Chassis_PowerLimit_PID.Init(0.01, 0.1, 0.01, 0,4, 0.4, 1);
}




/**
 * @brief 云台电机初始化函数
 * 
 * @param Gimbal_Motor 
 */
void Chassis_t::Gimbal_Init(LK_Motor_t* Gimbal_Motor1, RM_Motor_t* Gimbal_Motor2){
	// Chassis_t::Gimbal_Motor[Gimbal_BigYawMotor]   = Gimbal_Motor1;
	Chassis_t::LK_9025 = Gimbal_Motor1;
	// Chassis_t::Gimbal_Motor[Gimbal_SmallYawMotor] = Gimbal_Motor2;

	

    //Gimbal_Motor_PID[Gimbal_BigYawAngle].Init(10, 0 ,1, 0, 3, 4000, -1,0);
	//Gimbal_Motor_PID[Gimbal_BigYawSpeed].Init(150, 10 ,0, 0, 3, 6000,19000);

	Gimbal_Motor_PID[Gimbal_BigYawAngle].Init(20, 0 ,0, 0, 3, 100, -1,0);
	Gimbal_Motor_PID[Gimbal_BigYawSpeed].Init(4,0,120, 0, 3, 1000,2000); 
	

	LK_9025->SetZero_MechanicalAngle(BigGimbal_ZeroMechanicalAngle);
	SetGimbalTargetAngle(Machine,Position_Control,BigGimbal_ZeroMechanicalAngle);

	// Gimbal_Motor[Gimbal_SmallYaw]->SetZero_MechanicalAngle(Gimbal_Motor[Gimbal_SmallYaw]->GetAngle()); 

    // Gimbal_Zeropoint_Calibration[Gimbal_BigYawAngle].Init(0.1, 0, 0.2, 0, 2, 4000, -1,0);
	// Gimbal_Zeropoint_Calibration[Gimbal_BigYawSpeed].Init(100, 0.1, 0, 0, 2, 6000,19000);

	// Gimbal_Zeropoint_Calibration[Gimbal_BigYawAngle].Init(3, 0, 20, 0, 2, 4000, -1,0);
	// Gimbal_Zeropoint_Calibration[Gimbal_BigYawSpeed].Init(60, 0.01, 0, 0, 2, 6000,19000);

	//曹
	Gimbal_Zeropoint_Calibration[Gimbal_BigYawAngle].Init(10, 0, 17, 0, 4, 4000, 10000,0);
	Gimbal_Zeropoint_Calibration[Gimbal_BigYawSpeed].Init(30, 0.1, 0, 0, 4, 6000,15000,0);
	
}

/**
 * @brief 底盘小陀螺速度设置函数
 * @param Smallgyro_speed 小陀螺速度
 */
void Chassis_t::SetSmallgyroSpeed(float DR16_Smallgyro_Speed){
	DR16_Chassis_Smallgyro_speed = DR16_Smallgyro_Speed;
}


/**  
 * @brief 底盘与云台跟随模式累计角度差归零(开机启动时)
 * 
 */
void Chassis_t::GimbalAngle_Calibration_Start(void){
	uint64_t i = 2000;
	while (i)
	{

		Gimbal_Zeropoint_Calibration[Gimbal_BigYawAngle].GenerateRing(LK_9025->GetAngle(),LK_9025->GetZero_MechanicalAngle(),65535);
		Gimbal_Zeropoint_Calibration[Gimbal_BigYawSpeed].Generate(LK_9025->GetSpeed(),Gimbal_Zeropoint_Calibration[Gimbal_BigYawAngle].GetOutput());
		LK_9025->SetCurrent(Gimbal_Zeropoint_Calibration[Gimbal_BigYawSpeed].GetOutput());

		i--;
		osDelay(2);
	}
}

/**
 * @brief 底盘与云台跟随模式累计角度差归零(运动时)
 * 
 */
void Chassis_t::GimbalAngle_Calibration_Sport(void){
	uint64_t i = 500;
	while (i)
	{

		Gimbal_Zeropoint_Calibration[Gimbal_BigYawAngle].GenerateRing(LK_9025->GetAngle(),LK_9025->GetZero_MechanicalAngle(),65535);
		Gimbal_Zeropoint_Calibration[Gimbal_BigYawSpeed].Generate(LK_9025->GetSpeed(),Gimbal_Zeropoint_Calibration[Gimbal_BigYawAngle].GetOutput());
		LK_9025->SetCurrent(Gimbal_Zeropoint_Calibration[Gimbal_BigYawSpeed].GetOutput());

		// Chassis_Target_Speed.Z = -Chassis_Zeropoint_Calibration.GenerateRing(Chassis.Gimbal_Motor[Gimbal_BigYawMotor]->GetAngle(), Chassis.Gimbal_Motor[Gimbal_BigYawMotor]->GetZero_MechanicalAngle(), 8191);  
		// IK_MotorSpeed();
		// Calculate_MotorPID();


		i--;
		osDelay(2);
	}

	Lnitial_Angle_Deviation = Lnitial_Angle_Deviation + Chassis.Chassis_Presently_Attitude.Yaw_Angle - Chassis.Gimbal_Presently_Attitude.Yaw_Angle;

}

	
/**
 * @brief 底盘设置行为函数
 */
void Chassis_t::SetBehaviour(ChassisBehaviour_e Behaviour) {
	ChassisBehaviour = Behaviour;
}

/**
 * @brief 更新云台于底盘的相对姿态信息(陀螺仪)
 * 
 * @remark 同时完成了相对角度从360到(-pi,pi)的映射
 */
void Chassis_t::UpdataRelativeAttitude_Gyroscope(void){
	float RelativeAngle = Chassis_Presently_Attitude.Yaw_Angle - Gimbal_Presently_Attitude.Yaw_Angle;
	// if(abs(RelativeAngle) <= 3){RelativeAngle = 0;}

	float Gimbal_Angle_Yaw_pi = Gimbal_Presently_Attitude.Yaw_Angle * pi / 180.0f;
	float Gimbal_Angle_Pitch_pi = Gimbal_Presently_Attitude.Pitch_Angle * pi / 180.0f;

	Chassis_t::Yaw_RelativeAngularVelocity = - Chassis_Presently_Attitude.Z_Acceleration - Gimbal_Presently_Attitude.X_Acceleration * sin(Gimbal_Angle_Pitch_pi) + Gimbal_Presently_Attitude.Z_Acceleration * cos(Gimbal_Angle_Pitch_pi);
	Chassis_t::Pitch_RelativeAngularVelocity = - sin(Gimbal_Angle_Yaw_pi) * Chassis_Presently_Attitude.X_Acceleration - cos(Gimbal_Angle_Yaw_pi) * Chassis_Presently_Attitude.Y_Acceleration + Gimbal_Presently_Attitude.Y_Acceleration;

	Chassis_t::Yaw_RelativeAngle = -(float)((RelativeAngle + Kf * Chassis.Chassis_Currentspeed_Z) * pi / 180.0f);
}


/**
 * @brief 更新云台于底盘的相对姿态信息(机械角度)
 * 
 * @remark 同时完成了相对角度从360到(-pi,pi)的映射
 */
void Chassis_t::UpdataRelativeAttitude_Mechanical(void){


	RelativeAngle = (LK_9025->GetAngle() * 1.0f - LK_9025->GetZero_MechanicalAngle() * 1.0f);
	RelativeAngle = (RelativeAngle * 360.0f) / 65535.0f;
	if (RelativeAngle < -180)
	{
		RelativeAngle += 360;
	}
	else if (RelativeAngle > 180)
	{
		RelativeAngle -= 360;
	}
	RelativeAngle = (float)((RelativeAngle * pi / 180.0f));
    Chassis_t::Yaw_RelativeAngle = RelativeAngle;
	BigGimbal_ZeroMechanical = 0;

	// RelativeAngle_two = -(LK_9025->GetAngle() * 1.0f - (LK_9025->GetZero_MechanicalAngle() - 32768) * 1.0f);
	// RelativeAngle_two = (RelativeAngle_two * 360.0f) / 65535.0f;
	// if (RelativeAngle_two < -180)
	// {
	// 	RelativeAngle_two += 360;
	// }
	// else if (RelativeAngle_two > 180)
	// {
	// 	RelativeAngle_two -= 360;
	// }
	// RelativeAngle_two = (float)((RelativeAngle_two * pi / 180.0f));



	// //让小陀螺到底盘跟随切换同方向，不超功率

	// // if(chassis_spin_flag == 1){
	
	// // 	if(RelativeAngle_two <= 0){
	// // 		Chassis_t::Yaw_RelativeAngle = RelativeAngle_two;
	// // 		BigGimbal_ZeroMechanical = 1;
	// // 	}
	// // 	else{
	// // 		Chassis_t::Yaw_RelativeAngle = RelativeAngle;
	// // 		BigGimbal_ZeroMechanical = 0;
	// // 	}
	// // }

	// // else{
	// 	switch (BigGimbal_ZeroMechanical)
	// {
	// case 0:
	// 	if (abs(RelativeAngle) < 2.5f)
	//        {
	// 	    Chassis_t::Yaw_RelativeAngle = RelativeAngle;
	// 	    BigGimbal_ZeroMechanical = 0;
	// 	    }
	// 	    else
	// 	    {
	// 		    Chassis_t::Yaw_RelativeAngle = RelativeAngle_two;
	// 			BigGimbal_ZeroMechanical = 1;
	// 	    }
	// 	break;
	// case 1:
	// 	if (abs(RelativeAngle_two) < 2.5f)
	//     {
	// 	    Chassis_t::Yaw_RelativeAngle = RelativeAngle_two;
	// 	    BigGimbal_ZeroMechanical = 1;
	//     }
	// 	else
	// 	{
	// 		Chassis_t::Yaw_RelativeAngle = RelativeAngle;
	// 		BigGimbal_ZeroMechanical = 0;
	// 	}
	// 	break;
	// default:
	// 	break;
	// }
	// }
    // if(ChassisBehaviour == CHASSIS_SPIN){
	// 	if(Chassis_Target_Speed.Z > 800 && Chassis_Target_Speed.Z <= 1000){
	// 		Chassis_t::Yaw_RelativeAngle += Chassis_Target_Speed.Z * 0.00009f;  // 预测补偿
	// 	}
	// 	if(Chassis_Target_Speed.Z > 1000 && Chassis_Target_Speed.Z <= 1200){
	// 		Chassis_t::Yaw_RelativeAngle += Chassis_Target_Speed.Z * 0.0001f;  // 预测补偿
	// 	}
	// 	if(Chassis_Target_Speed.Z > 1200 && Chassis_Target_Speed.Z <= 1400){
	// 		Chassis_t::Yaw_RelativeAngle += Chassis_Target_Speed.Z * 0.00014f;  // 预测补偿
	// 	}
	// 	if(Chassis_Target_Speed.Z > 1400 && Chassis_Target_Speed.Z <= 1600){
	// 		Chassis_t::Yaw_RelativeAngle += Chassis_Target_Speed.Z * 0.00018f;  // 预测补偿
	// 	}
	// 	if(Chassis_Target_Speed.Z > 1600){
	// 		Chassis_t::Yaw_RelativeAngle += Chassis_Target_Speed.Z * 0.00022f;  // 预测补偿
	// 	}
	// }
	
}


/**
 * @brief 底盘功率计算
 * 
 * @remark 底盘功率计算
 * @param Watt 功率
 */
float Chassis_t::Chassis_Power_Calculation(void){ 
	float Motor_Power[4];
	Motor_Power[LU] = k0 + k1 * abs((float)Chassis_Motor[LU]->GetTorqueCurrent()) * 0.001f + k2 * abs((float)Chassis_Motor[LU]->GetSpeedRads()) + k3 * (float)Chassis_Motor[LU]->GetTorqueCurrent() * 0.001f * (float)Chassis_Motor[LU]->GetSpeedRads() + k4 * (float)Chassis_Motor[LU]->GetTorqueCurrent() * 0.001f * (float)Chassis_Motor[LU]->GetTorqueCurrent() * 0.001f + k5 * (float)Chassis_Motor[LU]->GetSpeedRads() * (float)Chassis_Motor[LU]->GetSpeedRads();
	Motor_Power[RU] = k0 + k1 * abs((float)Chassis_Motor[RU]->GetTorqueCurrent()) * 0.001f + k2 * abs((float)Chassis_Motor[RU]->GetSpeedRads()) + k3 * (float)Chassis_Motor[RU]->GetTorqueCurrent() * 0.001f * (float)Chassis_Motor[RU]->GetSpeedRads() + k4 * (float)Chassis_Motor[RU]->GetTorqueCurrent() * 0.001f * (float)Chassis_Motor[RU]->GetTorqueCurrent() * 0.001f + k5 * (float)Chassis_Motor[RU]->GetSpeedRads() * (float)Chassis_Motor[RU]->GetSpeedRads();
	Motor_Power[LD] = k0 + k1 * abs((float)Chassis_Motor[LD]->GetTorqueCurrent()) * 0.001f + k2 * abs((float)Chassis_Motor[LD]->GetSpeedRads()) + k3 * (float)Chassis_Motor[LD]->GetTorqueCurrent() * 0.001f * (float)Chassis_Motor[LD]->GetSpeedRads() + k4 * (float)Chassis_Motor[LD]->GetTorqueCurrent() * 0.001f * (float)Chassis_Motor[LD]->GetTorqueCurrent() * 0.001f + k5 * (float)Chassis_Motor[LD]->GetSpeedRads() * (float)Chassis_Motor[LD]->GetSpeedRads();
	Motor_Power[RD] = k0 + k1 * abs((float)Chassis_Motor[RD]->GetTorqueCurrent()) * 0.001f + k2 * abs((float)Chassis_Motor[RD]->GetSpeedRads()) + k3 * (float)Chassis_Motor[RD]->GetTorqueCurrent() * 0.001f * (float)Chassis_Motor[RD]->GetSpeedRads() + k4 * (float)Chassis_Motor[RD]->GetTorqueCurrent() * 0.001f * (float)Chassis_Motor[RD]->GetTorqueCurrent() * 0.001f + k5 * (float)Chassis_Motor[RD]->GetSpeedRads() * (float)Chassis_Motor[RD]->GetSpeedRads();
	Chassis_t::Chassis_CurrentPower = Motor_Power[LU] + Motor_Power[RU] + Motor_Power[LD] + Motor_Power[RD];
	return Chassis_t::Chassis_CurrentPower;
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
 * @brief Set云台的目标速度X,Y,Z(正方向分别为,上,左,逆时针)
 * 
 * @param Speed_X 
 * @param Speed_Y 
 * @param Speed_Z 
 */
void Chassis_t::SetGimbalTargetSpeed(float Speed_Z){
	Gimbal_Target_Speed.Z = Speed_Z;
}

/**
 * @brief Set底盘的目标速度X,Y,Z(正方向分别为,上,左,逆时针)
 * 
 * @param Speed_X  //物理中心X轴线速度
 * @param Speed_Y  //物理中心Y轴线速度
 * @param Speed_Z  //物理中心旋转角速度
 * 
 * @brief m/s
 */
void Chassis_t::SetChassisTargetSpeed(float Speed_X, float Speed_Y, float Speed_Z) {
	Chassis_Target_Speed.X = Speed_X;
	Chassis_Target_Speed.Y = Speed_Y;
	Chassis_Target_Speed.Z = Speed_Z;
}

/**
 * @brief 更新底盘坐标系相对大地坐标系的姿态信息
 * 
 * @param Yaw_Angle 
 * @param Pitch_Angle 
 * @param Yaw_Acceleration    
 * @param Pitch_Acceleration 
 */
void Chassis_t::UpdateChassisAttitude(float Yaw_Angle, float Pitch_Angle, float X_Acceleration, float Y_Acceleration, float Z_Acceleration){
	Chassis_Presently_Attitude.Yaw_Angle        = Yaw_Angle;
	Chassis_Presently_Attitude.Pitch_Angle      = Pitch_Angle;
	Chassis_Presently_Attitude.X_Acceleration   = X_Acceleration;
	Chassis_Presently_Attitude.Y_Acceleration   = Y_Acceleration;
	Chassis_Presently_Attitude.X_Acceleration   = X_Acceleration;
}

/**
 * @brief 更新云台坐标系相对大地坐标系的姿态信息
 * 
 * @param Yaw_Angle 
 * @param Pitch_Angle 
 * @param Yaw_Acceleration    
 * @param Pitch_Acceleration 
 */
void Chassis_t::UpdateGimbalAttitude(float Yaw_Angle, float Pitch_Angle, float X_Acceleration, float Y_Acceleration, float Z_Acceleration){
	Gimbal_Presently_Attitude.Yaw_Angle        = Yaw_Angle;
	Gimbal_Presently_Attitude.Pitch_Angle      = Pitch_Angle;
	Gimbal_Presently_Attitude.X_Acceleration   = X_Acceleration;
	Gimbal_Presently_Attitude.Y_Acceleration   = Y_Acceleration;
	Gimbal_Presently_Attitude.X_Acceleration   = X_Acceleration;
}


/**
 * @brief Get物理中心底盘的当前速度X,Y,Z(正方向分别为,上,左,逆时针)
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
	Motor_Target_Speed[LU] = (cos(LU_Angle)*Chassis_Target_Speed.X + sin(LU_Angle)*Chassis_Target_Speed.Y + Chassis_R*Chassis_Target_Speed.Z) / Motort_R;
	Motor_Target_Speed[RU] = (cos(RU_Angle)*Chassis_Target_Speed.X + sin(RU_Angle)*Chassis_Target_Speed.Y + Chassis_R*Chassis_Target_Speed.Z) / Motort_R;
	Motor_Target_Speed[LD] = (cos(LD_Angle)*Chassis_Target_Speed.X + sin(LD_Angle)*Chassis_Target_Speed.Y + Chassis_R*Chassis_Target_Speed.Z) / Motort_R;
	Motor_Target_Speed[RD] = (cos(RD_Angle)*Chassis_Target_Speed.X + sin(RD_Angle)*Chassis_Target_Speed.Y + Chassis_R*Chassis_Target_Speed.Z) / Motort_R;
	// clang-format on
}

/**
 * @brief 运动学正解算                         
 * 
 * @brief 计算出当前物理中心底盘的运动速度
 * @brief m/s
 */
void Chassis_t::FK_ChassisSpeed(void){
	Chassis_Currentspeed_X = 1.0f / 2.0f * (cos(LU_Angle)*Chassis_Motor[LU]->GetSpeed() + cos(RU_Angle)*Chassis_Motor[RU]->GetSpeed() + cos(LD_Angle)*Chassis_Motor[LD]->GetSpeed() + cos(RD_Angle)*Chassis_Motor[RD]->GetSpeed());
	Chassis_Currentspeed_Y = 1.0f / 2.0f * (sin(LU_Angle)*Chassis_Motor[LU]->GetSpeed() + sin(RU_Angle)*Chassis_Motor[RU]->GetSpeed() + sin(LD_Angle)*Chassis_Motor[LD]->GetSpeed() + sin(RD_Angle)*Chassis_Motor[RD]->GetSpeed());
	Chassis_Currentspeed_Z = 1.0f / 4.0f * (1.0f / Chassis_R * (Chassis_Motor[LU]->GetSpeed() + Chassis_Motor[RU]->GetSpeed() + Chassis_Motor[LD]->GetSpeed() + Chassis_Motor[RD]->GetSpeed()));
}

																																		

/**
 * @brief 与云台有相对角度时，计算速度向量（应用旋转矩阵，从云台坐标系变换到底盘坐标系）
 */
void Chassis_t::CalcSpeedWithRelativeAngle(void) {
	// clang-format off
	float last_x = Chassis_Target_Speed.X;
	float last_y = Chassis_Target_Speed.Y;
	if(BigGimbal_ZeroMechanical == 0){
		Chassis_Target_Speed.X = last_x *  cos(Yaw_RelativeAngle) - last_y * sin(Yaw_RelativeAngle);
	    Chassis_Target_Speed.Y = last_x *  sin(Yaw_RelativeAngle) + last_y * cos(Yaw_RelativeAngle);
	}
	if(BigGimbal_ZeroMechanical == 1){
		Chassis_Target_Speed.X = -last_x *  cos(Yaw_RelativeAngle) + last_y * sin(Yaw_RelativeAngle);
	    Chassis_Target_Speed.Y = -last_x *  sin(Yaw_RelativeAngle) - last_y * cos(Yaw_RelativeAngle);
	}
	
	// clang-format on
}

/**
 * @brief 更新云台陀螺仪累积误差
 * 
 * @param Error 
 */
void Chassis_t::UpadteGimbalAngleError(void){
    Chassis.Gimbal_Presently_Attitude.Yaw_Angle = Chassis.Gimbal_Presently_Attitude.Yaw_Angle + Lnitial_Angle_Deviation;
}

/**
 * @brief Set云台目标角度(-180,180)
 * 
 * @param MotorType 电机类型(Gimbal_BigYaw / Gimbal_SmallYaw)
 * @param Control_Type 云台控制模式(Position_Control / Speed_Control)
 * @param Target_Angle 目标角度(-180,180)
 * 
 */
void Chassis_t::SetGimbalTargetAngle(Control_type control_type,Gimbal_Control_Type Control_Type, float Target_Angle){
	switch (control_type)
	{
	case Imu:
		{
			switch(Control_Type)
			{
			case Position_Control:
				Gimbal_Target_Angle[Imu] = Target_Angle;
			    break;
            case Speed_Control:
			    Gimbal_Target_Angle[Imu] = Gimbal_Target_Angle[Imu] + (-Target_Angle/100.0f);
			    if(Gimbal_Target_Angle[Imu] < -180){
					Gimbal_Target_Angle[Imu] = 360.0f + Gimbal_Target_Angle[Imu];
				}
				else if(Gimbal_Target_Angle[Imu] > 180){
					Gimbal_Target_Angle[Imu] = -360.0f + Gimbal_Target_Angle[Imu];
				}
                break;
			default:
				break;
			}
				
		}
	    
    case Machine:
		Gimbal_Target_Angle[Machine] = Target_Angle;
		break;
	}
}

/**
 * @brief 云台自稳
 * 
 * @param MotorType 电机类型(Gimbal_BigYaw / Gimbal_SmallYaw)
 * 
 */
void Chassis_t::Gimbal_SelfStabilizing(Control_type control_type){
	// float Unit_Angle = 8191.0f / 360.0f;
	// float Lag = 1.0;
	// DR16_t* DR16 = DR16_Point();

	
    imu_t* imu = imuPoint();
	
	
	switch (control_type)
	{
	case Imu:
	    // if(DR16->SW1 == REMOTE_SW_MID){
		// 	Lag = 2.0;
		// }
		// else{
		// 	Lag = 1.0;
		// }
		// float BigYawTargetMechanicalAngle = Gimbal_Motor[Gimbal_BigYaw]->GetZero_MechanicalAngle() + (Gimbal_Target_Angle[Gimbal_BigYaw] - Chassis.Gimbal_Presently_Attitude.Yaw_Angle) * Unit_Angle;       //   TargetMechanicalAngle = (0,8191)


        LPF_Gimbal_speed.Init(3,10);

		LPF_Gimbal_speed.Generate(LK_9025->GetSpeed());
		// if(Chassis.Gimbal_Target_Speed.Z == 0){
			
		// }
		// else{
		// 	SetGimbalTargetAngle(Imu,Position_Control,imu->yaw);
		// }
        Gimbal_Motor_PID[Gimbal_BigYawAngle].GenerateRing(imu->yaw,Gimbal_Target_Angle[Imu],360);
        Gimbal_Motor_PID[Gimbal_BigYawSpeed].Generate(LPF_Gimbal_speed.GetOutput(),-Gimbal_Motor_PID[Gimbal_BigYawAngle].GetOutput() + Chassis.Gimbal_Target_Speed.Z );
	    LK_9025->SetCurrent((int16_t)Gimbal_Motor_PID[Gimbal_BigYawSpeed].GetOutput());



		break;
	
	case Machine:
	    Gimbal_Zeropoint_Calibration[Gimbal_BigYawAngle].GenerateRing(LK_9025->GetAngle(),Gimbal_Target_Angle[Machine],8191);
	    Gimbal_Zeropoint_Calibration[Gimbal_BigYawSpeed].Generate(LK_9025->GetSpeed(),Gimbal_Zeropoint_Calibration[Gimbal_BigYawAngle].GetOutput() - Chassis.Gimbal_Target_Speed.Z);

		LK_9025->SetCurrent(Gimbal_Zeropoint_Calibration[Gimbal_BigYawSpeed].GetOutput());
		SetGimbalTargetAngle(Machine,Position_Control,LK_9025->GetAngle());
		break;
	}
    
}

/**
 * @brief 重力前馈
 * 
 */
void Chassis_t::Chassis_Gravity_feed(void){
	IMU_t* IMU = BSP_IMU_Point();
     
    Chassis_Target_Speed.Y += - (IMU->Angle_Y + 0.9f) * Chassis_pitch_kf;
	Chassis_Target_Speed.X += - (IMU->Angle_X - 1.5f)  * Chassis_roll_kf;
}



/**
 * @brief 底盘电机运动
 * 
 */
void  Chassis_t::Calculate_MotorPID(void){
    float MotorSpeed_Max, SpeedLimitK = 1.0f, K_LimitByPwr = 1.0f;

    // 限速，保持各轮子之间的速度比
	// MotorSpeed_Max = 0;
	// for (int i = 0; i < 4; i++) {
	// 	if (fabs(Motor_Target_Speed[i]) > MotorSpeed_Max) {
	// 		MotorSpeed_Max = fabs(Motor_Target_Speed[i]);
	// 	}
	// }
	// if (MotorSpeed_Max > 8191) {
	// 	SpeedLimitK = 8191.0f / MotorSpeed_Max;
	// } else {
	// 	SpeedLimitK = 1.0f;
	// }

	// 计算速度环 PID
	Chasssis_Motor_PID[LU].Generate(Chassis_Motor[LU]->GetSpeed(),Motor_Target_Speed[LU]);
	Chasssis_Motor_PID[RU].Generate(Chassis_Motor[RU]->GetSpeed(),Motor_Target_Speed[RU]);
	Chasssis_Motor_PID[LD].Generate(Chassis_Motor[LD]->GetSpeed(),Motor_Target_Speed[LD]);
	Chasssis_Motor_PID[RD].Generate(Chassis_Motor[RD]->GetSpeed(),Motor_Target_Speed[RD]);


	float Motor_Power[4];
	// Motor_Power[LU] = k0 + k1 * abs(Chasssis_Motor_PID[LU].GetOutput() * 0.8f) * 0.001f + k2 * abs(Motor_Target_Speed[LU] * 0.104719775f) + k3 * Chasssis_Motor_PID[LU].GetOutput() * 0.8f * 0.001f * Motor_Target_Speed[LU] * 0.104719775f + k4 * Chasssis_Motor_PID[LU].GetOutput() * 0.8f * 0.001f * Chasssis_Motor_PID[LU].GetOutput() * 0.8f * 0.001f + k5 * Motor_Target_Speed[LU] * 0.104719775f * Motor_Target_Speed[LU] * 0.104719775f;
	// Motor_Power[RU] = k0 + k1 * abs(Chasssis_Motor_PID[RU].GetOutput() * 0.8f) * 0.001f + k2 * abs(Motor_Target_Speed[RU] * 0.104719775f) + k3 * Chasssis_Motor_PID[RU].GetOutput() * 0.8f * 0.001f * Motor_Target_Speed[RU] * 0.104719775f + k4 * Chasssis_Motor_PID[RU].GetOutput() * 0.8f * 0.001f * Chasssis_Motor_PID[RU].GetOutput() * 0.8f * 0.001f + k5 * Motor_Target_Speed[RU] * 0.104719775f * Motor_Target_Speed[RU] * 0.104719775f;
	// Motor_Power[LD] = k0 + k1 * abs(Chasssis_Motor_PID[LD].GetOutput() * 0.8f) * 0.001f + k2 * abs(Motor_Target_Speed[LD] * 0.104719775f) + k3 * Chasssis_Motor_PID[LD].GetOutput() * 0.8f * 0.001f * Motor_Target_Speed[LD] * 0.104719775f + k4 * Chasssis_Motor_PID[LD].GetOutput() * 0.8f * 0.001f * Chasssis_Motor_PID[LD].GetOutput() * 0.8f * 0.001f + k5 * Motor_Target_Speed[LD] * 0.104719775f * Motor_Target_Speed[LD] * 0.104719775f;
	// Motor_Power[RD] = k0 + k1 * abs(Chasssis_Motor_PID[RD].GetOutput() * 0.8f) * 0.001f + k2 * abs(Motor_Target_Speed[RD] * 0.104719775f) + k3 * Chasssis_Motor_PID[RD].GetOutput() * 0.8f * 0.001f * Motor_Target_Speed[RD] * 0.104719775f + k4 * Chasssis_Motor_PID[RD].GetOutput() * 0.8f * 0.001f * Chasssis_Motor_PID[RD].GetOutput() * 0.8f * 0.001f + k5 * Motor_Target_Speed[RD] * 0.104719775f * Motor_Target_Speed[RD] * 0.104719775f;

	Motor_Power[LU] = k0 + k1 * abs((float)Chassis_Motor[LU]->GetTorqueCurrent()) * 0.001f + k2 * abs(Motor_Target_Speed[LU] * 0.104719775f) + k3 * (float)Chassis_Motor[LU]->GetTorqueCurrent() * 0.001f * Motor_Target_Speed[LU] * 0.104719775f + k4 * (float)Chassis_Motor[LU]->GetTorqueCurrent() * 0.001f * (float)Chassis_Motor[LU]->GetTorqueCurrent() * 0.001f + k5 * Motor_Target_Speed[LU] * 0.104719775f * Motor_Target_Speed[LU] * 0.104719775f;
	Motor_Power[RU] = k0 + k1 * abs((float)Chassis_Motor[RU]->GetTorqueCurrent()) * 0.001f + k2 * abs(Motor_Target_Speed[RU] * 0.104719775f) + k3 * (float)Chassis_Motor[RU]->GetTorqueCurrent() * 0.001f * Motor_Target_Speed[RU] * 0.104719775f + k4 * (float)Chassis_Motor[RU]->GetTorqueCurrent() * 0.001f * (float)Chassis_Motor[RU]->GetTorqueCurrent() * 0.001f + k5 * Motor_Target_Speed[RU] * 0.104719775f * Motor_Target_Speed[RU] * 0.104719775f;
	Motor_Power[LD] = k0 + k1 * abs((float)Chassis_Motor[LD]->GetTorqueCurrent()) * 0.001f + k2 * abs(Motor_Target_Speed[LD] * 0.104719775f) + k3 * (float)Chassis_Motor[LD]->GetTorqueCurrent() * 0.001f * Motor_Target_Speed[LD] * 0.104719775f + k4 * (float)Chassis_Motor[LD]->GetTorqueCurrent() * 0.001f * (float)Chassis_Motor[LD]->GetTorqueCurrent() * 0.001f + k5 * Motor_Target_Speed[LD] * 0.104719775f * Motor_Target_Speed[LD] * 0.104719775f;
	Motor_Power[RD] = k0 + k1 * abs((float)Chassis_Motor[RD]->GetTorqueCurrent()) * 0.001f + k2 * abs(Motor_Target_Speed[RD] * 0.104719775f) + k3 * (float)Chassis_Motor[RD]->GetTorqueCurrent() * 0.001f * Motor_Target_Speed[RD] * 0.104719775f + k4 * (float)Chassis_Motor[RD]->GetTorqueCurrent() * 0.001f * (float)Chassis_Motor[RD]->GetTorqueCurrent() * 0.001f + k5 * Motor_Target_Speed[RD] * 0.104719775f * Motor_Target_Speed[RD] * 0.104719775f;

	Chassis_Future_Power = Motor_Power[LU] + Motor_Power[RU] + Motor_Power[LD] + Motor_Power[RD];

	float PowerLimit;
    Chassis_PowerLimit_PID.Generate(Chassis_Future_Power,Chassis_TargetPower);
	if (Chassis_PowerLimit_PID.GetOutput() < 0)
	{
		PowerLimit = 1 - abs(Chassis_PowerLimit_PID.GetOutput());
		
	}
	else{
		PowerLimit = abs(Chassis_PowerLimit_PID.GetOutput());
	}


	// Chassis_Power_Calculation();

	// *((uint32_t *)&Vofabuf[5]) = 0x7F800000U;
	// Vofabuf[0] = 100;
	// Vofabuf[1] = 80;
	// Vofabuf[2] = Chassis_Future_Power;
	// Vofabuf[3] = Chassis_CurrentPower;
	// BSP_UART_SendMessage(2,(uint8_t*)Vofabuf, sizeof(Vofabuf));

	// 将 PID 计算结果给电机
	Chassis_Motor[LU]->SetCurrent(Chasssis_Motor_PID[LU].GetOutput() * PowerLimit);  
	Chassis_Motor[RU]->SetCurrent(Chasssis_Motor_PID[RU].GetOutput() * PowerLimit);  
	Chassis_Motor[LD]->SetCurrent(Chasssis_Motor_PID[LD].GetOutput() * PowerLimit);  
	Chassis_Motor[RD]->SetCurrent(Chasssis_Motor_PID[RD].GetOutput() * PowerLimit);  

	// Chassis_Motor[LU]->SetCurrent(Chasssis_Motor_PID[LU].GetOutput());  
	// Chassis_Motor[RU]->SetCurrent(Chasssis_Motor_PID[RU].GetOutput());  
	// Chassis_Motor[LD]->SetCurrent(Chasssis_Motor_PID[LD].GetOutput());  
	// Chassis_Motor[RD]->SetCurrent(Chasssis_Motor_PID[RD].GetOutput());  


} 



/**
 * @brief 底盘计算
 * @caller ChassisTask
 */
void Chassis_t::Generate(void) {
	switch (ChassisBehaviour) {
		case CHASSIS_ZERO_FORCE:  // 底盘无力模式
		    Chassis_Motor[LU]->SetCurrent(0);
		    Chassis_Motor[RU]->SetCurrent(0);
		    Chassis_Motor[LD]->SetCurrent(0);
		    Chassis_Motor[RD]->SetCurrent(0);
			LK_9025->SetCurrent(0);
			ChassisBehaviour_Last = CHASSIS_ZERO_FORCE;
			return;                  // 无力的关键
			        
		case CHASSIS_NO_MOVE:  // 底盘不动模式
		    Motor_Target_Speed[LU] = 0;
		    Motor_Target_Speed[RU] = 0;
		    Motor_Target_Speed[LD] = 0;
		    Motor_Target_Speed[RD] = 0;
			ChassisBehaviour_Last = CHASSIS_NO_MOVE;
			break;

		case CHASSIS_NO_FOLLOW:                      // 底盘不跟随云台
		    CalcSpeedWithRelativeAngle();
			
			ChassisBehaviour_Last = CHASSIS_NO_FOLLOW;
			break;

		case CHASSIS_FOLLOW_GIMBAL:                  // 底盘跟随云台




		    //让小陀螺到底盘跟随切换同方向，不超功率

		    // if(ChassisBehaviour_Last == CHASSIS_SPIN){
			// 	chassis_spin_flag = 1;
			// 	time_spin = 0; 
			// }
			// if(chassis_spin_flag == 1){
			// 	time_spin++;
			// }
			
			// if(time_spin > 100 && chassis_spin_flag == 1){
			// 	chassis_spin_flag = 0;
			// }



			Chassis_Target_Speed.Z = Chassis_Follow_PID.GenerateRing(Yaw_RelativeAngle, 0, 2*pi);  // 跟随
			// FK_ChassisSpeed();
			CalcSpeedWithRelativeAngle();
				

			ChassisBehaviour_Last = CHASSIS_FOLLOW_GIMBAL;
			break;

		case CHASSIS_SPIN:                            // 小陀螺运动
		    Chassis_Target_Speed.Z = Smallgyro_speed + DR16_Chassis_Smallgyro_speed;
			// FK_ChassisSpeed();
			CalcSpeedWithRelativeAngle();
		
			ChassisBehaviour_Last = CHASSIS_SPIN;
			break;
	}

	if(ChassisBehaviour != CHASSIS_ZERO_FORCE){
		Chassis_Gravity_feed();
		IK_MotorSpeed();
	}
	Calculate_MotorPID();
}

/**
 * @brief 底盘线程
 */
void ChassisTask(void* argument) {
    
LK_Motor_t* LK_9025 = LK_Motor_Point();
	while (1) {
		if(Chassis.ChassisBehaviour == CHASSIS_SPIN || Chassis.ChassisBehaviour == CHASSIS_FOLLOW_GIMBAL || Chassis.ChassisBehaviour == CHASSIS_NO_FOLLOW){
			Chassis.Gimbal_SelfStabilizing(Imu);
		}

		
		Chassis.UpdataRelativeAttitude_Mechanical();
		Chassis.Generate();
		osDelay(6);
	}
}

osThreadId_t ChassisThreadHandle;
static const osThreadAttr_t ChassisTask_attributes = {"ChassisTask", 0, 0, 0, 0, 512, (osPriority_t)osPriorityNormal7};
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
