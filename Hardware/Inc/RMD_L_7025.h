/**
 * @file RMD_L_7025.h
 * @author jiang
 * @brief 7025电机头文件
 * @version 0.1
 * @date 2025-09-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#ifndef __RMD_L_7025_H__
#define __RMD_L_7025_H__

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



void RMD_L_7025_Init(void);

 
 #ifdef __cplusplus
 } /* extern "C" */
 #endif
 


#endif