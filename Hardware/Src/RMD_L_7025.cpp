/**
 * @file RMD_L_7025.cpp
 * @author jiang
 * @brief 7025电机源文件
 * @version 0.1
 * @date 2025-09-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#include "RMD_L_7025.h"

void RMD_L_7025_Callback(uint32_t ID, uint8_t* Data){


}

void RMD_L_7025Task(void *argument){

    BSP_CAN_SetRxCallbackFunc(1,RMD_L_7025_Callback);

    while (1)
    {

        // BSP_CAN_SendStandard8Data(1,0x200,Data);

       
        osDelay(1);
    }
    
}



static osThreadId_t RMD_L_7025ThreadID;
static const osThreadAttr_t RMD_L_7025Task_attributes = {"RMD_L_7025Task", 0, 0, 0, 0, 512, (osPriority_t)osPriorityNormal7};


void RMD_L_7025_Init(void){

    RMD_L_7025ThreadID = osThreadNew(RMD_L_7025Task, NULL, &RMD_L_7025Task_attributes);
    
}
