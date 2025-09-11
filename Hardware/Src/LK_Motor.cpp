/**
 * @file LK_Motor.cpp
 * @author jiang
 * @brief 领控电机
 * @version 0.1
 * @date 2025-09-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "LK_Motor.h"

uint8_t Struct_data[8] =  {0x9C,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t Current_data[8] = {0xA1,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

LK_Motor_t LK_9025;
osTimerId_t LK_Motor_osTimerId;

const static osTimerAttr_t LK_Motor_osTimerAttr = {"LK_Motor_osTimer", 0, NULL, 0};

/**
 * @brief 设置领控电机电流
 * 
 * @param iq (-2048~ 2048)
 */
void LK_Motor_t::SetCurrent(int16_t iq) {
    Current_data[4] = *(uint8_t *)(&iq);
    Current_data[5] = *(uint8_t *)(&iq) + 1;
    BSP_CAN_SendStandard8Data(1, 0x141,Current_data);
}

 void LK_CAN_Callback(uint32_t ID, uint8_t* Data) {
	// if (ID == 0x181) {
		if(Data[0] == 0x9C){
            memcpy(&LK_9025.Struct, Data, 8);
		}
	// }
}

void LK_Motor_osTimer_CbHandler(void *argument) {
    BSP_CAN_SendStandard8Data(1, 0x141,Struct_data);
}

 void LK_Motor_Init(void) {
	BSP_CAN_SetRxCallbackFunc(1, LK_CAN_Callback);
	LK_Motor_osTimerId = osTimerNew(LK_Motor_osTimer_CbHandler, osTimerPeriodic, NULL, &LK_Motor_osTimerAttr);
	osTimerStart(LK_Motor_osTimerId, 10);  // 4ms
}



LK_Motor_t* LK_Motor_Point(void) {
    return &LK_9025;
}