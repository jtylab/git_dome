/**
 * @file IMU_CAN.h
 * @author jiang
 * @brief C板之间的IMU姿态传感器信息传递
 * @version 0.1
 * @date 2025-04-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #ifndef IMU_CAN_H
 #define IMU_CAN_H

 #include "main.h"
 #include "BSP_CAN.h"
 #include "ChassisThread.h"


 #ifdef __cplusplus
 extern "C" {
 #endif

 #ifdef __cplusplus

 class Gimbal_IMU_t{

    public:
        
        float Yaw_Angle;
        float Pitch_Angle;
        float X_Acceleration;
        float Y_Acceleration;
        float Z_Acceleration;

        void UpdateAttitude(float Yaw_Angle, float Pitch_Angle, float X_Acceleration, float Y_Acceleration, float Z_Acceleration);
};


class Chassis_IMU_t{
    public:
        float Yaw_Angle;
        float Pitch_Angle;
        float X_Acceleration;
        float Y_Acceleration;
        float Z_Acceleration; 

        void UpdateAttitude(float Yaw_Angle, float Pitch_Angle, float X_Acceleration, float Y_Acceleration, float Z_Acceleration);
};

class IMU_CAN_t{
    private:
        uint32_t IMU_CAN_ID;

    public:
        void SetCANID(uint32_t ID);
        uint32_t GetCANID(void);
        
};

static Chassis_IMU_t Chassis_IMU;
static Gimbal_IMU_t Gimbal_IMU;
static float Rx_Data[5];


void IMU_CAN_Init(void);

 #endif

 #ifdef __cplusplus
 } /* extern "C" */
 #endif
 
 #endif // !IMU_CAN_H
