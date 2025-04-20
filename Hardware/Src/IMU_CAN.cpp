/**
 * @file IMU_CAN.cpp
 * @author jiang
 * @brief C板之间的IMU姿态传感器信息传递
 * @version 0.1
 * @date 2025-04-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #include "IMU_CAN.h"


 #define IMU_CAN_Port 1
 #define CAN_ID   1

 void IMU_CAN_t::IMU_CAN_Callback(uint32_t ID, uint8_t* Data){
 }


/**
 * @brief 设置IMU_CAN_ID
 * 
 * @param ID 
 */
 void IMU_CAN_t::SetCANID(uint32_t ID){
    IMU_CAN_ID = ID;
 }

 void IMU_CAN_t::IMU_CAN_Init(void){
    SetCANID(CAN_ID);
    BSP_CAN_SetRxCallbackFunc(IMU_CAN_Port,IMU_CAN_Callback);
 }

 