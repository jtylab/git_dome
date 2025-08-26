/**
 * @file DM_IMU_CAN.h
 * @author jiang
 * @brief 达妙IMU
 * @version 0.1
 * @date 2025-08-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #ifndef DM_IMU_CAN_H
 #define DM_IMU_CAN_H


 #include "main.h"
 #include "BSP_CAN.h"
#include "spi.h"

 


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

#include "can.h"


 #ifdef __cplusplus
 extern "C" {
 #endif


#include "dm_imu.h"

void DM_IMU_CAN_Init(void);




 #ifdef __cplusplus
 } /* extern "C" */
 #endif
 

 
 #endif // !1