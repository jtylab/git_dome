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
 #define CAN_ID  0x300

 IMU_CAN_t IMU_CAN_Rx;

 float Rx_Data[5];

void Gimbal_IMU_t::UpdateAttitude(float Yaw_Angle, float Pitch_Angle, float X_Acceleration, float Y_Acceleration, float Z_Acceleration){
	Gimbal_IMU_t::Yaw_Angle        = Yaw_Angle;
	Gimbal_IMU_t::Pitch_Angle      = Pitch_Angle;
	Gimbal_IMU_t::X_Acceleration   = X_Acceleration;
	Gimbal_IMU_t::Y_Acceleration   = Y_Acceleration;
	Gimbal_IMU_t::Z_Acceleration   = Z_Acceleration;
}


void Chassis_IMU_t::UpdateAttitude(float Yaw_Angle, float Pitch_Angle, float X_Acceleration, float Y_Acceleration, float Z_Acceleration){
	Chassis_IMU_t::Yaw_Angle        = Yaw_Angle;
	Chassis_IMU_t::Pitch_Angle      = Pitch_Angle;
	Chassis_IMU_t::X_Acceleration   = X_Acceleration;
	Chassis_IMU_t::Y_Acceleration   = Y_Acceleration;
	Chassis_IMU_t::Z_Acceleration   = Z_Acceleration;
}

 /**
  * @brief 接收回调函数
  * 
  * @param ID 
  * @param Data 
  */
 void IMU_CAN_Callback(uint32_t ID, uint8_t* Data){
	if(sizeof(Data) == 20){
		// Gimbal_IMU.Yaw_Angle      = Data[ 0]<<24 | Data[ 1]<<16 | Data[ 2]<<8 | Data[ 3];
	    // Gimbal_IMU.Pitch_Angle    = Data[ 4]<<24 | Data[ 5]<<16 | Data[ 6]<<8 | Data[ 7];
	    // Gimbal_IMU.X_Acceleration = Data[ 8]<<24 | Data[ 9]<<16 | Data[10]<<8 | Data[11];
	    // Gimbal_IMU.Y_Acceleration = Data[12]<<24 | Data[13]<<16 | Data[14]<<8 | Data[15];
	    // Gimbal_IMU.Z_Acceleration = Data[16]<<24 | Data[17]<<16 | Data[18]<<8 | Data[19];
		Rx_Data[0] = Data[ 0]<<24 | Data[ 1]<<16 | Data[ 2]<<8 | Data[ 3];
	    Rx_Data[1] = Data[ 4]<<24 | Data[ 5]<<16 | Data[ 6]<<8 | Data[ 7];
	    Rx_Data[2] = Data[ 8]<<24 | Data[ 9]<<16 | Data[10]<<8 | Data[11];
	    Rx_Data[3] = Data[12]<<24 | Data[13]<<16 | Data[14]<<8 | Data[15];
	    Rx_Data[4] = Data[16]<<24 | Data[17]<<16 | Data[18]<<8 | Data[19];

		Chassis.UpdateGimbalAttitude(Rx_Data[0], Rx_Data[1], Rx_Data[2], Rx_Data[3], Rx_Data[4]);
	}
 }

 
/**
 * @brief GetCANID
 * 
 * @return uint32_t 
 */
uint32_t IMU_CAN_t::GetCANID(void){
  return IMU_CAN_ID;
}

/**
 * @brief 设置IMU_CAN_ID
 * 
 * @param ID 
 */
 void IMU_CAN_t::SetCANID(uint32_t ID){
    IMU_CAN_ID = ID;
 }

 void IMU_CAN_Init(void){
  IMU_CAN_Rx.SetCANID(CAN_ID);
  BSP_CAN_SetRxCallbackFunc(IMU_CAN_Port,IMU_CAN_Callback);
 }

 