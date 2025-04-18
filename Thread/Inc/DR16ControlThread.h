/**
 * @file DR16ControlThread.h
 * @author jiang
 * @brief DR16遥控器控制线程
 * @version 0.1
 * @date 2025-04-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #ifndef DR16ControlThread_H
 #define DR16ControlThread_H

 #include "main.h"
 #include "FreeRTOS.h"
 #include "cmsis_os.h"
 #include "task.h"
 #include "math.h" 

 #include "DR16.h"

 #include "ChassisThread.h"


 #define Scaler_Chassis 1000.0f 





 #ifdef __cplusplus
 extern "C" {
 #endif


static TaskHandle_t DR16ControlThread_Handle = NULL;
void DR16ControlThread_Init();


 #ifdef __cplusplus
 }
#endif



 #endif // !DR16ControlThread_H
