/**
 * @file C_Board_CAN.h
 * @author jiang
 * @brief 双C板通信
 * @version 0.1
 * @date 2025-07-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #ifndef C_Board_CAN_H
 #define C_Board_CAN_H


 #include "main.h"
#include "main.h"
#include "BSP_CAN.h"
#include "main.h"
#include "spi.h"
#include "tim.h"
		
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"	
#include "RM_Motor.h"

#include "ChassisThread.h"
#include "Robodefine.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct CAN_RxData_t{
	uint8_t ChassisBehaviour;
    float Gimbal_Target_Speed_X;
    float Gimbal_Target_Speed_Y;
    float Gimbal_Target_Speed_Z;
    float Gimbal_Target_Angle;
}CAN_RxData_t;



typedef struct CAN_TxData_t{
	float Yaw_RelativeAngle;
}CAN_TxData_t;



__attribute__((aligned(4)))  CAN_RxData_t Chassis_CAN_RxData;
__attribute__((aligned(4)))  CAN_TxData_t Chassis_CAN_TxData;

static osThreadId_t Chassis_C_CANThreadID;

#ifdef __cplusplus
} /* extern "C" */
#endif

 
 #endif // !1