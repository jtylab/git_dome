/**
 * @file LK_Motor.h
 * @author jiang
 * @brief 领控电机
 * @version 0.1
 * @date 2025-09-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __LK_MOTOR_H__
#define __LK_MOTOR_H__
#include "main.h"
#include "BSP_CAN.h"

#include "Basic.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "string.h"

 #ifdef __cplusplus
 extern "C" {
 #endif

 #ifdef __cplusplus

 struct Struct_t
 {
    uint8_t Head;
    int8_t temperature;
    int16_t iq;
    int16_t speed;
    uint16_t encoder;
 };

 class LK_Motor_t{
    public:
        Struct_t Struct;
        uint16_t MechanicalAngle;

        
   void SetZero_MechanicalAngle(uint16_t MechanicalAngle);
   uint16_t GetZero_MechanicalAngle(void);

   void SetCurrent(int16_t iq);
   void SetSpeed(int32_t speed);
   void SetAngle(uint8_t Direction , uint32_t Angle);
   void SetPID(uint8_t p, uint8_t i);
   uint16_t GetAngle(void);
   int16_t GetSpeed(void);
 };


 void LK_Motor_Init(void);
 LK_Motor_t* LK_Motor_Point(void);


 #endif

 #ifdef __cplusplus
 } /* extern "C" */
 #endif
 

 
 #endif // !1