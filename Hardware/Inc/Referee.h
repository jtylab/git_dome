/**
 * @file Referee.h
 * @brief RoboMaster 裁判系统通信 v1.6.1
 * @attention
 * @hardware 
 * @version 1.6.1-alpha
 * @date 2024-03-23
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __Referee_H
#define __Referee_H

#include "main.h"

#include "Referee_Protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)
typedef struct {
	Ref_GameStatus_t GameStatus;
	Ref_HP_t HP;
	Ref_Event_t Event;
	Ref_SupplyAction_t SupplyAction;
	Ref_RobotStatus_t RobotStatus;
	Ref_PowerHeat_t PowerHeat;
	Ref_RobotPos_t Pos;
	Ref_Buff_t Buff;
	Ref_AirSupport_t AirSupport;
	Ref_GotDamage_t GotDamage;
	Ref_Shooting_t Shooting;
	Ref_Allowance_t Allowance;
	Ref_RfidStatus_t RfidStatus;
	Ref_DartClientCmd_t DartClientCmd;
	Ref_GroundRobotPos_t GroundRobotPos;
	Ref_RadarMarking_t RadarMarking;
	Ref_SentryInfo_t SentryInfo;
	Ref_RadarInfo_t RadarInfo;
	uint8_t RobotID_Last;
	uint16_t Chassis_Power_Limit_Last;
	bool Updated_RobotStatus; 
	bool isOnline;
} Referee_t;
#pragma pack()

void Referee_Init(void);
void Referee_Send(Ref_Header_t *header, uint16_t *CRC16);
void Referee_SetExchHeader(Ref_ExchData_Header_t *header);
Referee_t *Referee_Point(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __Referee_H */

/*********************************END OF FILE*********************************/
