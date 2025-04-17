/**
 * @file RefereeByCAN_ChassisSide.cpp
 * @brief RoboMaster 裁判系统通信
 * @attention
 * @hardware
 * @version 
 * @date 2024-03-24
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "RefereeByCAN_ChassisSide.h"

#include "BSP_CAN.h"

#include "Referee.h"

#define RefByCAN_CAN_Port 2

#pragma pack(1)
typedef struct {
		uint8_t Robot_ID;
		uint8_t Robot_Level;
		uint16_t Shooter_Barrel_Cooling_Value;
		uint16_t Shooter_Barrel_Heat_Limit;
		uint16_t Chassis_Power_Limit;
} RefByCAN_RobotStatus_t;

typedef struct {
		uint16_t Chassis_Voltage;
		uint16_t Chassis_Current;
		float Chassis_Power;
} RefByCAN_ChassisPower_t;

typedef struct {
		uint16_t Chassis_Power_Buffer;
		uint16_t Shooter_17mm_1_Barrel_Heat;
		uint16_t Shooter_17mm_2_Barrel_Heat;
		uint16_t Shooter_42mm_Barrel_Heat;
} RefByCAN_Heat_t;
#pragma pack()

__attribute__((aligned(4))) static RefByCAN_RobotStatus_t Tx_RobotStatus;
__attribute__((aligned(4))) static RefByCAN_ChassisPower_t Tx_ChassisPower;
__attribute__((aligned(4))) static RefByCAN_Heat_t Tx_Heat;

extern Referee_t Referee;

void RefByCAN_RobotStatus_TxCbHandler(void) {
    Tx_RobotStatus.Robot_ID = Referee.RobotStatus.Robot_ID;
    Tx_RobotStatus.Robot_Level = Referee.RobotStatus.Robot_Level;
    Tx_RobotStatus.Shooter_Barrel_Cooling_Value = Referee.RobotStatus.Shooter_Barrel_Cooling_Value;
    Tx_RobotStatus.Shooter_Barrel_Heat_Limit = Referee.RobotStatus.Shooter_Barrel_Heat_Limit;
    Tx_RobotStatus.Chassis_Power_Limit = Referee.RobotStatus.Chassis_Power_Limit;
    BSP_CAN_SendStandard8Data(RefByCAN_CAN_Port, 0x500UL, (uint8_t *)&Tx_RobotStatus);
}

void RefByCAN_ChassisPower_TxCbHandler(void) {
    Tx_ChassisPower.Chassis_Voltage = Referee.PowerHeat.Chassis_Voltage;
    Tx_ChassisPower.Chassis_Current = Referee.PowerHeat.Chassis_Current;
    Tx_ChassisPower.Chassis_Power = Referee.PowerHeat.Chassis_Power;
    BSP_CAN_SendStandard8Data(RefByCAN_CAN_Port, 0x501UL, (uint8_t *)&Tx_ChassisPower);
}

void RefByCAN_Heat_TxCbHandler(void) {
    Tx_Heat.Chassis_Power_Buffer = Referee.PowerHeat.Chassis_Power_Buffer;
    Tx_Heat.Shooter_17mm_1_Barrel_Heat = Referee.PowerHeat.Shooter_17mm_1_Barrel_Heat;
    Tx_Heat.Shooter_17mm_2_Barrel_Heat = Referee.PowerHeat.Shooter_17mm_2_Barrel_Heat;
    Tx_Heat.Shooter_42mm_Barrel_Heat = Referee.PowerHeat.Shooter_42mm_Barrel_Heat;
    BSP_CAN_SendStandard8Data(RefByCAN_CAN_Port, 0x502UL, (uint8_t *)&Tx_Heat);
}

/*********************************END OF FILE*********************************/