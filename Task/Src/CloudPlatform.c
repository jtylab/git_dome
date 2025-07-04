/**
 * @file CloudPlatform.c
 * @author jiang
 * @brief 云台
 * @version 0.1
 * @date 2025-07-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */


 #include "CloudPlatform.h"

 void CloudPlatformTask(void *argument){
    while (1)
    {
        /* code */
    }
 }

 osThreadId_t CloudPlatformTask_Handle;
 osThreadAttr_t CloudPlatformTask_attributes = {"CloudPlatformTask", 0, 0, 0, 0, 2048, (osPriority_t)osPriorityRealtime1};

 void CloudPlatformTask_Init(void){
    taskENTER_CRITICAL();

    CloudPlatformTask_Handle = osThreadNew(CloudPlatformTask, NULL, &CloudPlatformTask_attributes);

    taskEXIT_CRITICAL();
 }