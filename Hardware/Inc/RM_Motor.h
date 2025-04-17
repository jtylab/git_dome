/**
 * @file RM_Motor.h
 * @brief 大疆 RoboMaster 系列电机
 * @attention
 * @version 0.1
 * @date 2024-03-22
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __RM_Motor_H
#define __RM_Motor_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
class RM_Motor_CAN;
class RM_Motor_t {
	public:
		enum MType {
			M3508 = 0,
			M2006 = 0,
			GM6020
		};
		enum OutputType {
			Voltage = 0,
			Current  // 6020 电流模式不能和 3508 一起用
		};
	private:
		uint8_t prv_CAN_Port;
		MType prv_Type;
		uint8_t prv_ID;

		int16_t prv_TorqueCurrent;  // 实际转矩电流 I_e ，其中 U_e * I_e = T_e * Ω 
		int16_t prv_Speed;          // 转子转速 n (RPM)
		uint16_t prv_Angle;         // 转子机械角度 0~8191 (0°~360°)
		uint8_t prv_Temp;           // 电机温度
		// int16_t prv_RecvNull;

		uint16_t prv_Angle_Last;

		float prv_Speed_rads;    // 转子转速 Ω (rad/s)
		int32_t prv_Round;       // 转子旋转步路
		int32_t prv_TotalAngle;  // 转子总角度

		int16_t prv_CurrentSetting;  // 电调电流设定值
		OutputType prv_OutputType;

	public:
		RM_Motor_t();
		RM_Motor_t(uint8_t CAN_Port, MType Type, uint8_t ID, OutputType OutputType = Current);
		uint8_t Init(void);
		uint8_t Init(uint8_t CAN_Port, MType Type, uint8_t ID, OutputType OutputType = Current);

		int16_t GetTorqueCurrent(void);
		int16_t GetSpeed(void);
		uint16_t GetAngle(void);
		uint8_t GetTemp(void);

		float GetSpeedRads(void);
		int32_t GetRound(void);
		void ClearRound(void);

		void SetCurrent(int16_t Current);

		void RxHandler(uint8_t *RxData);

		friend class RM_Motor_CAN;
};

class RM_Motor_CAN {
	private:
		uint8_t prv_CAN_Port;

		RM_Motor_t *prv_Motors[12];
		
		struct {
			//                      M3508  | GM6020  | GM6020
			//                     Current | Voltage | Current
			int16_t ID_200[4];  // 1 2 3 4 |         | 
			int16_t ID_1FF[4];  // 5 6 7 8 | 1 2 3 4 |        
			int16_t ID_2FF[4];  //         | 5 6 7 N |        
			int16_t ID_1FE[4];  //         |         | 1 2 3 4
			int16_t ID_2FE[4];  //         |         | 5 6 7 N
		} TxSlots;
	public:
		RM_Motor_CAN(uint8_t CAN_Port) {
			prv_CAN_Port = CAN_Port;
		}
		uint8_t GetPort(void);
		uint8_t Registe(RM_Motor_t *Motor);
		void Send(void);
		void RxHandler(/*uint8_t Port,*/ uint32_t ID, uint8_t* RxData);
};

#endif

void RM_Motor_Init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __RM_Motor_H */

/*********************************END OF FILE*********************************/
