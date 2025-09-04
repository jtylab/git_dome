/**
 * @file Robodefine.h
 * @author jiang
 * @brief 宏定义参数
 * @version 0.1
 * @date 2025-07-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#ifndef Robodefine_H
#define Robodefine_H


#define LU 0
#define RU 1
#define LD 2
#define RD 3

#define PI 3.1415926535897932384626f

#define LU_Angle    1.0f/4.0f*PI                  //底盘电机于物理中心的角度
#define RU_Angle   -1.0f/4.0f*PI
#define LD_Angle    3.0f/4.0f*PI
#define RD_Angle   -3.0f/4.0f*PI

#define Chassis_R   0.25f                         //???????????????????  底盘中心到电机的半径(需要测量)C   
#define Motort_R    0.075f                        //???????????????????  电机中心到轮子的半径(需要测量)

#define Acceldeviation_X  -0.01f                 //IMU静止时的加速度偏差
#define Acceldeviation_Y  0.6f
#define Acceldeviation_Z  -9.7f


#define Gimbal_BigYawAngle    0  
#define Gimbal_BigYawSpeed    1        //BigYaw云台6020电机PID

#define Gimbal_SmallYawAngle    2  
#define Gimbal_SmallYawSpeed    3      //SmallYaw云台6020电机PID



#define Kf        0.0007f                       //底盘相对角度前馈可以使小陀螺模式时做到X，Y线速度方向不变
#define Smallgyro_speed     800.0f             //小陀螺转速
#define BigGimbal_ZeroMechanicalAngle  5000      //机械零点(0~8091)


#define Scaler_Chassis 400.0f  //DR16遥控器灵敏度

#define C_Board_CAN_Port 2                  //双C板通信CAN端口号
#define C_Board_CAN_ID (uint32_t)0x123UL    //双C板通信CAN ID
#define DM_IMU__CAN_ID (uint32_t)0x321UL    //

#define C_Board_CAN_ThreadFlag1 0x01      //双C板通信线程任务通知Flag



#endif // !

