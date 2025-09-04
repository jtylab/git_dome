/**
 * @file MainThread.cpp
 * @brief 主线程
 * @attention
 * @hardware 
 * @version 0.1
 * @date 2021-11-21
 * @author tianxu, jiang
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "MainThread.h"



uint16_t time = 0;


RM_Motor_t ChassisMotor_LU(1, RM_Motor_t::M3508, 1);
RM_Motor_t ChassisMotor_RU(1, RM_Motor_t::M3508, 2);
RM_Motor_t ChassisMotor_LD(1, RM_Motor_t::M3508, 3);
RM_Motor_t ChassisMotor_RD(1, RM_Motor_t::M3508, 4);

RM_Motor_t GimbalMotor_BigYaw(1, RM_Motor_t::GM6020,2);
RM_Motor_t GimbalMotor_smallYaw(1, RM_Motor_t::GM6020,3);


float Vofabuf[4]={0};
/**
 * @brief 主线程函数
 */
void MainTask(void *argument) {
    BSP_IMU_Start(-0.0041016,0.00223481,-0.000858274);

    Chassis_t* Chassis = ChassisPoint();
	DR16_t* DR16 = DR16_Point();
	imu_t*  imu = imuPoint();
	PID_Data_t* PID_Data = GetPID_DataPoint();
// 	Booster_t *Booster = BoosterPoint();

	Chassis->Chassis_Init(&ChassisMotor_LU, &ChassisMotor_RU, &ChassisMotor_LD, &ChassisMotor_RD);
	Chassis->Gimbal_Init(&GimbalMotor_BigYaw,&GimbalMotor_smallYaw);

    *((uint32_t *)&Vofabuf[3]) = 0x7F800000U;
	osDelay(3500);
	vofa_usart_Init();

    LPF_t LPF_imu,LPF_Gimbal_speed;
	LPF_imu.Init(3,10);
	LPF_Gimbal_speed.Init(3,10);
	
	while (1) {
		LPF_imu.Generate(imu->yaw);
		LPF_Gimbal_speed.Generate(Chassis->Gimbal_Motor[Gimbal_BigYawMotor]->GetSpeed());

		// Vofabuf[0] = Chassis->Gimbal_Target_Speed.Z;
		// Vofabuf[1] = LPF_Gimbal_speed.GetOutput();
		// Vofabuf[2] = Chassis->Gimbal_Motor[Gimbal_BigYawMotor]->GetSpeed();

		// Vofabuf[0] = Chassis->Gimbal_Target_Angle[Imu];
		// Vofabuf[1] = LPF_imu.GetOutput();
		// Vofabuf[2] = imu->yaw;
		

		Chassis->UpdataRelativeAttitude_Mechanical();

		if(Chassis->ChassisBehaviour == CHASSIS_SPIN || Chassis->ChassisBehaviour == CHASSIS_FOLLOW_GIMBAL){
			Chassis->Gimbal_SelfStabilizing(Imu);
		}
		if (Chassis->ChassisBehaviour == CHASSIS_NO_FOLLOW)
		{
			Chassis->Gimbal_SelfStabilizing(Imu);
		}


		// Chassis->Gimbal_Motor_PID[Gimbal_BigYawAngle].SetPID(PID_Data->kp,PID_Data->ki,PID_Data->kd);


	    BSP_UART_SendMessage(2, (uint8_t *)Vofabuf, sizeof(Vofabuf));



		// if(time == 1500){
		// 	Chassis->SetGimbalTargetAngle(Imu,Position_Control,30);
		// }
		// if(time == 3000){
		// 	Chassis->SetGimbalTargetAngle(Imu,Position_Control,90);
		// }
		// if(time == 4500){
		// 	Chassis->SetGimbalTargetAngle(Imu,Position_Control,150);
		// 	time = 0;
		// }

		// if(time == 500){
		// 	Chassis->SetGimbalTargetSpeed(0);
		// }
		// if(time == 1500){
		// 	Chassis->SetGimbalTargetSpeed(30);
		// }
		// if(time == 2000){
		// 	Chassis->SetGimbalTargetSpeed(60);
		// 	time = 0;
		// }

		time++;
			
		 
		
		 osDelay(3);
	}
}

osThreadId_t MainThreadHandle;
static const osThreadAttr_t MainTask_attributes = {"MainTask", 0, 0, 0, 0, 2048, (osPriority_t)osPriorityNormal7};
/**
 * @brief 线程初始化
 */
void MainThread_Init(void) {
	MainThreadHandle = osThreadNew(MainTask, NULL, &MainTask_attributes);  // 创建线程
}

/**********************************END OF FILE**********************************/
