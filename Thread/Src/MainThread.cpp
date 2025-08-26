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



RM_Motor_t ChassisMotor_LU(1, RM_Motor_t::M3508, 1);
RM_Motor_t ChassisMotor_RU(1, RM_Motor_t::M3508, 2);
RM_Motor_t ChassisMotor_LD(1, RM_Motor_t::M3508, 3);
RM_Motor_t ChassisMotor_RD(1, RM_Motor_t::M3508, 4);

RM_Motor_t GimbalMotor_BigYaw(1, RM_Motor_t::GM6020,6);
RM_Motor_t GimbalMotor_smallYaw(1, RM_Motor_t::GM6020,2);



/**
 * @brief 主线程函数
 */
void MainTask(void *argument) {
    BSP_IMU_Start(-0.0041016,0.00223481,-0.000858274);

    Chassis_t* Chassis = ChassisPoint();
	DR16_t* DR16 = DR16_Point();
// 	Booster_t *Booster = BoosterPoint();

	Chassis->Chassis_Init(&ChassisMotor_LU, &ChassisMotor_RU, &ChassisMotor_LD, &ChassisMotor_RD);
	Chassis->Gimbal_Init(&GimbalMotor_BigYaw,&GimbalMotor_smallYaw);

	osDelay(3500);
	
	while (1) {

		Chassis->UpdataRelativeAttitude_Mechanical();

		if(Chassis->ChassisBehaviour == CHASSIS_SPIN){
			Chassis->Gimbal_SelfStabilizing(Imu);
		}
		if (Chassis->ChassisBehaviour == CHASSIS_FOLLOW_GIMBAL)
		{
			Chassis->Gimbal_SelfStabilizing(Machine);
		}
		
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
