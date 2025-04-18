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


RM_Motor_t ChassisMotor_1(1, RM_Motor_t::M3508, 1);
RM_Motor_t ChassisMotor_2(1, RM_Motor_t::M3508, 2);
RM_Motor_t ChassisMotor_3(1, RM_Motor_t::M3508, 3);
RM_Motor_t ChassisMotor_4(1, RM_Motor_t::M3508, 4);

/**
 * @brief 开启DR16遥控器控制函数
 * 
 */
void DR16Control_Start(void){
	xTaskNotifyGive(DR16ControlThread_Handle);
}

/**
 * @brief 开启自动控制
 * 
 */
void Automaticcontrol_Start(void){

}

/**
 * @brief 控制方式设置函数
 * 
 * @param Mode 
 */
void Set_Controlmode(Control_mode Mode){
	switch (Mode)
	{
	case DR16_CONTROL:
        DR16Control_Start();
		break;
	
	case AUTOMATIC_CONTROL:
        Automaticcontrol_Start();
	    break;
	}
}


/**
 * @brief 主线程函数
 */
void MainTask(void *argument) {
    BSP_IMU_Start(-0.0041016,0.00223481,-0.000858274);
	

    
// 	Booster_t *Booster = BoosterPoint();

	// float K_Gimbal = 1.0;
// 	bool Boost_Press_Last = 0;
    static Chassis_t *Chassis = ChassisPoint();
	Chassis->Init(&ChassisMotor_1, &ChassisMotor_2, &ChassisMotor_3, &ChassisMotor_4);

// 	//Gimbal->Init(&GimbalMotor_Yaw, 0x06A0, &GimbalMotor_Pitch, 0x0DBE, IMU_CAN_Point(2));  // Ax=0,21:0x0091,22:0x14EC  Ax=1.86,21:0x0135,22:0x1444
    
    

	osDelay(3500);
	// while(DR16ControlThread_Handle != NULL){
	// 	xTaskNotifyGive(DR16ControlThread_Handle);
	// }
	
	// Set_Controlmode(DR16_CONTROL);
	
	while (1) {

		
		// xTaskNotify(DR16ControlThread_Handle,0,eNoAction);
		 osDelay(4);
	}
}

osThreadId_t MainThreadHandle;
static const osThreadAttr_t MainTask_attributes = {"MainTask", 0, 0, 0, 0, 2048, (osPriority_t)osPriorityNormal};
/**
 * @brief 线程初始化
 */
void MainThread_Init(void) {
	MainThreadHandle = osThreadNew(MainTask, NULL, &MainTask_attributes);  // 创建线程
}

/**********************************END OF FILE**********************************/
