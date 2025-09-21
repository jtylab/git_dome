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
uint8_t PID_Wdata[8] =  {0x32,0x00,0x32,0x32,0x32,0x32,0x32,0x32};
uint8_t Current_data[8] = {0xA1,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t Speed_data[8] = {0xA2,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t Angle_data[8] = {0xA5,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t PID_Rdata[8] =  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t PID_Rdata_R[8] =  {0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

LK_Motor_t LK_9025;
osTimerId_t LK_Motor_osTimerId;

const static osTimerAttr_t LK_Motor_osTimerAttr = {"LK_Motor_osTimer", 0, NULL, 0};

/**
 * @brief 设置领控电机电流
 * 
 * @param iq (-2048~ 2048)
 */
void LK_Motor_t::SetCurrent(int16_t iq) {

	iq = (int16_t)LimitBoth(iq, 2000, -2000);
    Current_data[4] = *(uint8_t *)(&iq);
    Current_data[5] = *((uint8_t *)(&iq) + 1);
    BSP_CAN_SendStandard8Data(2, 0x141,Current_data);
}

void LK_Motor_t::SetSpeed(int32_t speed){
	speed *= 100;
	Speed_data[4] = *(uint8_t *)(&speed);
	Speed_data[5] = *((uint8_t *)(&speed)+1);
	Speed_data[6] = *((uint8_t *)(&speed)+2);
	Speed_data[7] = *((uint8_t *)(&speed)+3);
	BSP_CAN_SendStandard8Data(2, 0x141,Speed_data);
}

void LK_Motor_t::SetAngle(uint8_t Direction , uint32_t Angle){
    Angle *= 100;

	Angle_data[1] = Direction;

	Angle_data[4] = *(uint8_t *)(&Angle);
	Angle_data[5] = *((uint8_t *)(&Angle)+1);
	Angle_data[6] = *((uint8_t *)(&Angle)+2);
	Angle_data[7] = *((uint8_t *)(&Angle)+3);
	BSP_CAN_SendStandard8Data(2, 0x141,Angle_data);
}

void LK_Motor_t::SetPID(uint8_t p, uint8_t i){
	PID_Wdata[2] = p;
	PID_Wdata[3] = i;
	BSP_CAN_SendStandard8Data(2, 0x141,PID_Wdata);
}

uint16_t LK_Motor_t::GetAngle(void){
	return LK_9025.Struct.encoder;
}

int16_t LK_Motor_t::GetSpeed(void){
	return LK_9025.Struct.speed;
}

uint16_t LK_Motor_t::GetZero_MechanicalAngle(void){
	return LK_9025.MechanicalAngle;
}

void LK_Motor_t::SetZero_MechanicalAngle(uint16_t MechanicalAngle){
	LK_9025.MechanicalAngle = MechanicalAngle;
}

 void LK_CAN_Callback(uint32_t ID, uint8_t* Data) {
	if (ID == 0x141) {
		if(Data[0] == 0x9C){
            memcpy(&LK_9025.Struct, Data, 8);
		}
	}
}

void LK_Motor_osTimer_CbHandler(void *argument) {
    BSP_CAN_SendStandard8Data(2, 0x141,Struct_data);
}

 void LK_Motor_Init(void) {
	BSP_CAN_SetRxCallbackFunc(2, LK_CAN_Callback);
	LK_Motor_osTimerId = osTimerNew(LK_Motor_osTimer_CbHandler, osTimerPeriodic, NULL, &LK_Motor_osTimerAttr);
	osTimerStart(LK_Motor_osTimerId, 4); 
}



LK_Motor_t* LK_Motor_Point(void) {
    return &LK_9025;
}