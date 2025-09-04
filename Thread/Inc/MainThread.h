/**
 * @file MainThread.h
 * @brief 主线程
 * @attention
 * @hardware 
 * @version 0.1
 * @date 2021-07-25
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __MainThread_H
#define __MainThread_H

#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"


#include "RM_Motor.h"
#include "MainThread.h"

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
#include "InteractionThread.h"
#include "Robodefine.h"
#include "vofa_usart.h"

#ifdef __cplusplus
extern "C" {
#endif

enum Control_mode{
    DR16_CONTROL = 0,     //遥控器控制
    AUTOMATIC_CONTROL,    //自动控制
};

void MainThread_Init(void);


#ifdef __cplusplus
} /* extern "C" */
#endif





#endif /* __MainThread_H */

/*********************************END OF FILE*********************************/
