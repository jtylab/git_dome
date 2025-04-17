/**
 * @file Referee_Protocol.cpp
 * @brief RM裁判系统通信通信协议 v1.6.1
 * @attention
 * @hardware
 * @version 1.6.1-alpha
 * @date 2024-03-23
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "Referee_Protocol.h"

#include "CRC.h"

/**
 * @brief 校验帧头数据
 * @param header 帧头指针
 * @return bool 是否通过校验
 * @retval 0 校验失败
 * @retval 1 校验通过
 */
bool Ref_CheckCRC_Header(Ref_Header_t *header) {
	if (Get_CRC8(&header->SOF, sizeof(Ref_Header_t) - 1) == header->CRC8)
		return 1;
	else
		return 0;
}

/**
 * @brief 校验整包数据
 * @param frame 帧指针
 * @return bool 是否通过校验
 * @retval 0 校验失败
 * @retval 1 校验通过
 */
bool Ref_CheckCRC_Frame(Ref_t *frame) {
	if (Get_CRC16(&frame->Header.SOF, sizeof(Ref_Header_t) + 2 + frame->Header.Len) == *(uint16_t *)(frame->Data + frame->Header.Len))
		return 1;
	else
		return 0;
}

/**
 * @brief 计算帧头CRC校验
 * @param header 帧头指针
 */
void Ref_CalcCRC_Header(Ref_Header_t *header) {
	header->CRC8 = Get_CRC8(&header->SOF, sizeof(Ref_Header_t) - 1);
}

/**
 * @brief 计算帧头CRC校验
 * @param SOF 帧头 SOF 指针
 * @param Data_Len Data 的长度（不包括 frame_tail）
 * @param CRC16 CRC 校验指针
 */
void Ref_CalcCRC_Frame(uint8_t *SOF, uint16_t Data_Len, uint16_t *CRC16) {
	*CRC16 = Get_CRC16(SOF, sizeof(Ref_Header_t) + 2 + Data_Len);
}

/**
 * @brief 由机器人ID得到客户端ID
 * @param RobotID 机器人ID
 * @return uint16_t 客户端 ID
 */
uint16_t Ref_GetClientFromRobot(uint8_t RobotID) {
	switch (RobotID) {
		case Ref_RobotID_R_Hero:
			return Ref_ClientID_R_Hero;
		case Ref_RobotID_R_Engineer:
			return Ref_ClientID_R_Engineer;
		case Ref_RobotID_R_Infantry1:
			return Ref_ClientID_R_Infantry1;
		case Ref_RobotID_R_Infantry2:
			return Ref_ClientID_R_Infantry2;
		case Ref_RobotID_R_Infantry3:
			return Ref_ClientID_R_Infantry3;
		case Ref_RobotID_R_Drone:
			return Ref_ClientID_R_Drone;
		case Ref_RobotID_B_Hero:
			return Ref_ClientID_B_Hero;
		case Ref_RobotID_B_Engineer:
			return Ref_ClientID_B_Engineer;
		case Ref_RobotID_B_Infantry1:
			return Ref_ClientID_B_Infantry1;
		case Ref_RobotID_B_Infantry2:
			return Ref_ClientID_B_Infantry2;
		case Ref_RobotID_B_Infantry3:
			return Ref_ClientID_B_Infantry3;
		case Ref_RobotID_B_Drone:
			return Ref_ClientID_B_Drone;
		default:
			return 0;
	}
}

/*********************************END OF FILE*********************************/
