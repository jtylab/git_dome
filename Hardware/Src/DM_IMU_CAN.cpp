/**
 * @file DM_IMU_CAN.cpp
 * @author jiang
 * @brief 达妙IMU
 * @version 0.1
 * @date 2025-08-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#include "DM_IMU_CAN.h"



float x,y;
uint8_t reg = 1;
void DM_IMU_CAN_Callback(uint32_t ID, uint8_t* Data){
    if(ID == 0x11){
        // memcpy(DM_IMU_Data,Data,sizeof(Data));
        IMU_UpdateData(Data);

    }

}

void DM_IMU_CANTask(void *argument){
    imu_t* imu = imuPoint();

    BSP_CAN_SetRxCallbackFunc(2,DM_IMU_CAN_Callback);

    while (1)
    {
        IMU_RequestData(&hcan2,0x01,reg);
        reg++;
        if(reg > 3)reg = 1;

        
       
        osDelay(1);
    }
    
}



static osThreadId_t DM_IMU_CANThreadID;
static const osThreadAttr_t DM_IMU_CANTask_attributes = {"DM_IMU_CANTask", 0, 0, 0, 0, 512, (osPriority_t)osPriorityNormal7};


void DM_IMU_CAN_Init(void){

    DM_IMU_CANThreadID = osThreadNew(DM_IMU_CANTask, NULL, &DM_IMU_CANTask_attributes);
    
}
