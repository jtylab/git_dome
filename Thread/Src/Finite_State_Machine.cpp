/**
 * @file Finite_State_Machine.cpp
 * @author jiang
 * @brief 有限状态机(更新周期10Hz)
 * @version 0.1
 * @date 2025-09-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Finite_State_Machine.h"



 void Finite_State_Machine_Task(void *argument){
    Chassis_t* Chassis = ChassisPoint();



    while (1)
    {
        Chassis->Chassis_Power_Calculation();

        osDelay(10);
    }
 }




osThreadId_t Finite_State_Machine_Handle;
static const osThreadAttr_t Finite_State_Machine_attributes = {"Finite_State_Machine_Task", 0, 0, 0, 0, 512, (osPriority_t)osPriorityNormal7};
/**
 * @brief 线程初始化
 */
void Finite_State_Machine_Init(void) {
	Finite_State_Machine_Handle = osThreadNew(Finite_State_Machine_Task, NULL, &Finite_State_Machine_attributes);  // 创建线程
}