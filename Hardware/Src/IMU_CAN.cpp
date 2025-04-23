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


 #define IMU_CAN_Port 2
 #define CAN_ID  (uint32_t)0x123UL

 IMU_CAN_t IMU_CAN_Rx;


//  static float Rx_Data[5];
//  static float A;

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
	if(ID == CAN_ID){ 
		Chassis_t* Chassis = ChassisPoint();
        memcpy(&Chassis->Gimbal_Presently_Attitude.Yaw_Angle,&Data[0],4);
		memcpy(&Chassis->Gimbal_Presently_Attitude.Pitch_Angle,&Data[4],4);
		memcpy(&Gimbal_IMU.X_Acceleration,&Data[8],4);
		memcpy(&Gimbal_IMU.Y_Acceleration,&Data[12],4);
		memcpy(&Gimbal_IMU.Z_Acceleration,&Data[16],4);
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

 