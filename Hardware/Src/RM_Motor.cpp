/**
 * @file RM_Motor.cpp
 * @brief 大疆 RoboMaster 系列电机
 * @attention 依赖 BSP_CAN
 * @hardware
 * @version 0.1
 * @date 2024-03-22
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "RM_Motor.h"

#include "BSP_CAN.h"

#include "Basic.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"

#define RM_Motor_CAN_NUM 1

static RM_Motor_CAN CAN_Bridge[RM_Motor_CAN_NUM] = {1};

osTimerId_t RM_Motor_osTimerId;
const static osTimerAttr_t RM_Motor_osTimerAttr = {"RM_Motor_osTimer", 0, NULL, 0};
static void RM_Motor_osTimer_CbHandler(void *argument);
static void RM_Motor_CAN_RxCbHandler(uint8_t Port, uint32_t ID, uint8_t *Data);  // uint8_t Port,

void RM_Motor_Init(void) {
	// BSP_CAN_SetRxAllCallbackFunc(RM_Motor_CAN_RxCbHandler);
	BSP_CAN_SetRxCallbackFunc(1, [](uint32_t ID, uint8_t* Data){ CAN_Bridge[0].RxHandler(ID, Data); });
	// BSP_CAN_SetRxCallbackFunc(2, [](uint32_t ID, uint8_t *Data) { CAN_Bridge[0].RxHandler(ID, Data); });
	RM_Motor_osTimerId = osTimerNew(RM_Motor_osTimer_CbHandler, osTimerPeriodic, NULL, &RM_Motor_osTimerAttr);
	osTimerStart(RM_Motor_osTimerId, 4);  // 4ms
}

RM_Motor_t::RM_Motor_t(void) {
	prv_CAN_Port = 0;
	prv_Type = M3508;
	prv_ID = 255;

	prv_TorqueCurrent = 0;
	prv_Speed = 0;
	prv_Angle = 0;
	prv_Temp = 0;
	// prv_RecvNull = 0;

	prv_Angle_Last = 0;

	prv_Speed_rads = 0.0f;
	prv_Round = 0;
	prv_TotalAngle = 0;

	prv_CurrentSetting = 0;

	prv_OutputType = Current;
}

RM_Motor_t::RM_Motor_t(uint8_t CAN_Port, MType Type, uint8_t ID, OutputType OutputType) {
	RM_Motor_t();
	prv_CAN_Port = CAN_Port;
	prv_Type = Type;
	prv_ID = ID;

	if (Type == GM6020) {
		prv_OutputType = Voltage;  // OutputType
	} else {
		prv_OutputType = Current;
	}
	Init();
}

uint8_t RM_Motor_t::Init(void) {
	// if (prv_CAN_Port == 0 || prv_CAN_Port > RM_Motor_CAN_NUM) return 1;
	// return CAN_Bridge[prv_CAN_Port - 1].Registe(this);
	for (int i = 0; i < RM_Motor_CAN_NUM; i++) {
		if (CAN_Bridge[i].GetPort() == prv_CAN_Port) {
			return CAN_Bridge[i].Registe(this);
		}
	}
	return 1;
}

uint8_t RM_Motor_t::Init(uint8_t CAN_Port, MType Type, uint8_t ID, OutputType OutputType) {
	prv_CAN_Port = CAN_Port;
	prv_Type = Type;
	prv_ID = ID;

	if (Type == GM6020) {
		prv_OutputType = Voltage;  // OutputType
	} else {
		prv_OutputType = Current;
	}

	return Init();
}

/**
 * @brief 获取电机转矩电流
 */
int16_t RM_Motor_t::GetTorqueCurrent(void) {
	return prv_TorqueCurrent;
}

/**
 * @brief 获取电机转子速度
 * @return 电机转子速度(RPM)
 */
int16_t RM_Motor_t::GetSpeed(void) {
	return prv_Speed;
}

/**
 * @brief 获取电机机械角度
 * @return 电机机械角度 0~8191 (0°~360°)
 */
uint16_t RM_Motor_t::GetAngle(void) {
	return prv_Angle;
}

/**
 * @brief 获取电机转子温度
 * @return 电机转子温度(℃)
 */
uint8_t RM_Motor_t::GetTemp(void) {
	return prv_Temp;
}

/**
 * @brief 获取电机转子速度
 * @return 电机转子速度 (rad/s)
 */
float RM_Motor_t::GetSpeedRads(void) {
	return prv_Speed_rads;
}

/**
 * @brief 获取电机转动步路
 */
int32_t RM_Motor_t::GetRound(void) {
	return prv_Round;
}

/**
 * @brief 电机转动步路清零
 */
void RM_Motor_t::ClearRound(void) {
	prv_Round = 0;
}

/**
 * @brief 设置电机的电调输出值
 */
void RM_Motor_t::SetCurrent(int16_t Current) {
	prv_CurrentSetting = Current;
}

void RM_Motor_t::RxHandler(uint8_t *RxData) {  // clang-format off
	prv_Angle         = (RxData[0] << 8) + RxData[1];
	prv_Speed         = (RxData[2] << 8) + RxData[3];
	prv_TorqueCurrent = (RxData[4] << 8) + RxData[5];  
	prv_Temp          =  RxData[6];
	// prv_RecvNull      =  RxData[7];
	// clang-format on
	if (prv_Angle - prv_Angle_Last > 4096) {
		prv_Round--;
	} else if (prv_Angle - prv_Angle_Last < -4096) {
		prv_Round++;
	}
	prv_Angle_Last = prv_Angle;
	prv_Speed_rads = pi * (float)prv_Speed / 30.0f;
}

uint8_t RM_Motor_CAN::GetPort(void) {
	return prv_CAN_Port;
}
uint8_t RM_Motor_CAN::Registe(RM_Motor_t *Motor) {
	switch (Motor->prv_Type) {
		case RM_Motor_t::M3508:
			if (Motor->prv_ID > 8) return 1;
			if (prv_Motors[Motor->prv_ID - 1]) return 1;
			prv_Motors[Motor->prv_ID - 1] = Motor;
			break;
		case RM_Motor_t::GM6020:
			if (Motor->prv_ID > 7) return 1;
			if (prv_Motors[Motor->prv_ID + 4 - 1]) return 1;
			prv_Motors[Motor->prv_ID + 4 - 1] = Motor;
			break;
		default:
			return 1;
	}
	return 0;
}

void RM_Motor_CAN::Send(void) {
	RM_Motor_t *pMotor = NULL;
	uint8_t TxSlot_Active = 0;
	for (int i = 0; i < 4; i++) {
		TxSlots.ID_200[i] = 0;
		pMotor = prv_Motors[i];
		if (!pMotor) continue;
		TxSlots.ID_200[i] = (pMotor->prv_CurrentSetting << 8) + (pMotor->prv_CurrentSetting >> 8);
		TxSlot_Active |= 0x01;
	}
	for (int i = 0; i < 4; i++) {
		TxSlots.ID_1FF[i] = 0;
		pMotor = prv_Motors[i + 4];
		if (!pMotor) continue;
		if (pMotor->prv_Type == RM_Motor_t::GM6020 && pMotor->prv_OutputType == RM_Motor_t::Current) continue;
		TxSlots.ID_1FF[i] = (pMotor->prv_CurrentSetting << 8) + (pMotor->prv_CurrentSetting >> 8);
		TxSlot_Active |= 0x02;
	}
	for (int i = 0; i < 4; i++) {
		TxSlots.ID_2FF[i] = 0;
		pMotor = prv_Motors[i + 8];
		if (!pMotor) continue;
		if (pMotor->prv_Type != RM_Motor_t::GM6020) continue;
		if (pMotor->prv_OutputType == RM_Motor_t::Current) continue;
		TxSlots.ID_2FF[i] = (pMotor->prv_CurrentSetting << 8) + (pMotor->prv_CurrentSetting >> 8);
		TxSlot_Active |= 0x04;
	}
	for (int i = 0; i < 4; i++) {
		TxSlots.ID_1FE[i] = 0;
		pMotor = prv_Motors[i + 4];
		if (!pMotor) continue;
		if (pMotor->prv_Type != RM_Motor_t::GM6020) continue;
		if (pMotor->prv_OutputType == RM_Motor_t::Voltage) continue;
		TxSlots.ID_1FE[i] = (pMotor->prv_CurrentSetting << 8) + (pMotor->prv_CurrentSetting >> 8);
		TxSlot_Active |= 0x08;
	}
	for (int i = 0; i < 4; i++) {
		TxSlots.ID_2FE[i] = 0;
		pMotor = prv_Motors[i + 8];
		if (!pMotor) continue;
		if (pMotor->prv_Type != RM_Motor_t::GM6020) continue;
		if (pMotor->prv_OutputType == RM_Motor_t::Voltage) continue;
		TxSlots.ID_2FE[i] = (pMotor->prv_CurrentSetting << 8) + (pMotor->prv_CurrentSetting >> 8);
		TxSlot_Active |= 0x10;
	}
	// TxSlot_Active = 0;
	if (TxSlot_Active & 0x01) BSP_CAN_SendStandard8Data(prv_CAN_Port, 0x200, (uint8_t *)TxSlots.ID_200);
	if (TxSlot_Active & 0x02) BSP_CAN_SendStandard8Data(prv_CAN_Port, 0x1FF, (uint8_t *)TxSlots.ID_1FF);
	if (TxSlot_Active & 0x04) BSP_CAN_SendStandard8Data(prv_CAN_Port, 0x2FF, (uint8_t *)TxSlots.ID_2FF);
	if (TxSlot_Active & 0x08) BSP_CAN_SendStandard8Data(prv_CAN_Port, 0x1FE, (uint8_t *)TxSlots.ID_1FE);
	if (TxSlot_Active & 0x10) BSP_CAN_SendStandard8Data(prv_CAN_Port, 0x2FE, (uint8_t *)TxSlots.ID_2FE);
}

void RM_Motor_CAN::RxHandler(/*uint8_t Port,*/ uint32_t ID, uint8_t *RxData) {
	uint8_t idx = ID - 0x200 - 1;
	// if (Port != prv_CAN_Port) return;
	if (ID < 0x201 || ID > 0x20C) return;
	if (!prv_Motors[idx]) return;
	prv_Motors[idx]->RxHandler(RxData);
}

void RM_Motor_osTimer_CbHandler(void *argument) {
	for (uint8_t i = 0; i < RM_Motor_CAN_NUM; i++) {
		CAN_Bridge[i].Send();
	}
}

/**
 * @brief 电机CAN接收回调函数
 */
void RM_Motor_CAN_RxCbHandler(uint8_t Port, uint32_t ID, uint8_t *Data) {  // uint8_t Port,
	// if (ID < 0x201 || ID > 0x20C) return;
	for (int i = 0; i < RM_Motor_CAN_NUM; i++) {
		if (CAN_Bridge[i].GetPort() == Port) CAN_Bridge[i].RxHandler(ID, Data);
		// CAN_Bridge[i].RxHandler(Port, ID, Data);
	}
}

/*********************************END OF FILE*********************************/
