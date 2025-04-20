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


class IMU_CAN_t{
    private:
        uint32_t IMU_CAN_ID;

    public:
        void IMU_CAN_Init(void)
        void IMU_CAN_Callback(uint32_t ID, uint8_t* Data);
        void SetCANID(uint32_t ID);
        
};



 #ifdef __cplusplus
 extern "C" {
 #endif

 #ifdef __cplusplus















 #endif

 #ifdef __cplusplus
 } /* extern "C" */
 #endif
 
 #endif // !IMU_CAN_H
