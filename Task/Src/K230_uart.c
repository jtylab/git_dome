/**
 * @file K230_uart.c
 * @author jiang
 * @brief 视觉通信
 * @version 0.1
 * @date 2025-07-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #include "K230_Uart.h"

 void K230_UartTask(void *argument){
    while (1)
    {
        /* code */
    }
 }

 osThreadId_t K230_UartTask_Handle;
 osThreadAttr_t K230_UartTask_attributes = {"K230_UartTask", 0, 0, 0, 0, 2048, (osPriority_t)osPriorityRealtime1};

void K230_UartTask_Init(void){
    taskENTER_CRITICAL();

    K230_UartTask_Handle = osThreadNew(K230_UartTask, NULL, &K230_UartTask_attributes);

    taskEXIT_CRITICAL();
}