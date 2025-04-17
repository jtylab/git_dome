/**
 * @file Referee.cpp
 * @brief RoboMaster 裁判系统通信 v1.6.1
 * @attention
 * @hardware
 * @version 1.6.1-alpha
 * @date 2024-03-23
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "Referee.h"

#include "BSP_UART.h"

#include "RefereeByCAN_ChassisSide.h"

#include "math.h"
#include "string.h"  // memcpy

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "timers.h"

#define Referee_UART_Port 2

Referee_t Referee = {0};

#pragma pack(1)

#pragma pack()

osTimerId_t Referee_Online_osTimerId;
osTimerId_t Referee_RoboStatus_osTimerId;
osThreadId_t RefereeThreadHandle;
const static osTimerAttr_t Referee_Online_osTimerAttr = {"Referee_Online_osTimer", 0, NULL, 0};
const static osTimerAttr_t Referee_RobotStatus_osTimerAttr = {"Referee_RoboStatus_osTimer", 0, NULL, 0};
static const osThreadAttr_t RefereeTask_attributes = {"RefereeTask", 0, 0, 0, 0, 256, (osPriority_t)osPriorityNormal};
static void Referee_UART_RxCbHandler(uint8_t *Data, uint32_t Len);
static void Referee_Online_osTimer_CbHandler(void *argument);
static void Referee_RobotStatus_osTimer_CbHandler(void *argument);
void Referee_Task(void *argument);
/**
 * @brief Referee 初始化
 */
void Referee_Init(void) {
	Referee.RobotStatus.Robot_ID = 0;
	Referee.RobotID_Last = 0;
	Referee.Chassis_Power_Limit_Last = 0;
	BSP_UART_SetBaudrate(Referee_UART_Port, 115200);
	BSP_UART_SetRxCallbackFunc(Referee_UART_Port, Referee_UART_RxCbHandler);
	Referee_Online_osTimerId = osTimerNew(Referee_Online_osTimer_CbHandler, osTimerPeriodic, NULL, &Referee_Online_osTimerAttr);
	Referee_RoboStatus_osTimerId = osTimerNew(Referee_RobotStatus_osTimer_CbHandler, osTimerPeriodic, NULL, &Referee_RobotStatus_osTimerAttr);
	RefereeThreadHandle = osThreadNew(Referee_Task, NULL, &RefereeTask_attributes);
	osTimerStart(Referee_Online_osTimerId, 100);      // 100ms
	osTimerStart(Referee_RoboStatus_osTimerId, 300);  // 300ms
}

/**
 * @brief 数据处理
 * @param header 帧头 Head 指针
 */
static void Referee_DataParser(Ref_t *frame) {
	/**
	 * @note 在调用xTimerResetFromISR()函数时，如果该函数导致定时器服务/守护任务离开Blocked状态（即有新的消息到达定时器命令队列），
	 *       并且该任务的优先级大于或等于当前执行任务（即被中断的任务）的优先级，则在xTimerResetFromISR()函数内部
	 *       会将pxHigherPriorityTaskWoken指向的变量设置为pdTRUE。如果xTimerResetFromISR()函数将该值设置为pdTRUE，
	 *       则在中断退出之前应进行上下文切换。
	 */
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	switch (frame->CMD_ID) {
		case Ref_CMDID_GameStatus:
			if (frame->Header.Len == sizeof(Ref_GameStatus_t)) {
				memcpy(&Referee.GameStatus, frame->Data, sizeof(Ref_GameStatus_t));
				Referee.isOnline = 1;
			}
			break;
		case Ref_CMDID_HP:
			if (frame->Header.Len == sizeof(Ref_HP_t)) {
				memcpy(&Referee.HP, frame->Data, sizeof(Ref_HP_t));
				Referee.isOnline = 1;
			}
			break;
		case Ref_CMDID_Event:
			if (frame->Header.Len == sizeof(Ref_Event_t)) {
				memcpy(&Referee.Event, frame->Data, sizeof(Ref_Event_t));
				Referee.isOnline = 1;
			}
			break;
		case Ref_CMDID_SupplyAction:
			if (frame->Header.Len == sizeof(Ref_SupplyAction_t)) {
				memcpy(&Referee.SupplyAction, frame->Data, sizeof(Ref_SupplyAction_t));
				Referee.isOnline = 1;
			}
			break;
		case Ref_CMDID_RobotStatus:
			if (frame->Header.Len == sizeof(Ref_RobotStatus_t)) {
				memcpy(&Referee.RobotStatus, frame->Data, sizeof(Ref_RobotStatus_t));
				if (Referee.RobotStatus.Robot_ID)
					Referee.RobotID_Last = Referee.RobotStatus.Robot_ID;
				if (Referee.RobotStatus.Chassis_Power_Limit)
					Referee.Chassis_Power_Limit_Last = Referee.RobotStatus.Chassis_Power_Limit;
				RefByCAN_RobotStatus_TxCbHandler();
				xTimerResetFromISR((TimerHandle_t)Referee_RoboStatus_osTimerId, &xHigherPriorityTaskWoken);
				Referee.Updated_RobotStatus = 1;
				Referee.isOnline = 1;
			}
			break;
		case Ref_CMDID_PowerHeat:
			if (frame->Header.Len == sizeof(Ref_PowerHeat_t)) {
				memcpy(&Referee.PowerHeat, frame->Data, sizeof(Ref_PowerHeat_t));
				RefByCAN_ChassisPower_TxCbHandler();
				RefByCAN_Heat_TxCbHandler();
				Referee.isOnline = 1;
			}
			break;
		case Ref_CMDID_RobotPos:
			if (frame->Header.Len == sizeof(Ref_RobotPos_t)) {
				memcpy(&Referee.Pos, frame->Data, sizeof(Ref_RobotPos_t));
				Referee.isOnline = 1;
			}
			break;
		case Ref_CMDID_Buff:
			if (frame->Header.Len == sizeof(Ref_Buff_t)) {
				memcpy(&Referee.Buff, frame->Data, sizeof(Ref_Buff_t));
				Referee.isOnline = 1;
			}
			break;
		case Ref_CMDID_AirSupport:
			if (frame->Header.Len == sizeof(Ref_AirSupport_t)) {
				memcpy(&Referee.AirSupport, frame->Data, sizeof(Ref_AirSupport_t));
				Referee.isOnline = 1;
			}
			break;
		case Ref_CMDID_GotDamage:
			if (frame->Header.Len == sizeof(Ref_GotDamage_t)) {
				memcpy(&Referee.GotDamage, frame->Data, sizeof(Ref_GotDamage_t));
				Referee.isOnline = 1;
			}
			break;
		case Ref_CMDID_Shooting:
			if (frame->Header.Len == sizeof(Ref_Shooting_t)) {
				memcpy(&Referee.Shooting, frame->Data, sizeof(Ref_Shooting_t));
				Referee.isOnline = 1;
			}
			break;
		case Ref_CMDID_Allowance:
			if (frame->Header.Len == sizeof(Ref_Allowance_t)) {
				memcpy(&Referee.Allowance, frame->Data, sizeof(Ref_Allowance_t));
				Referee.isOnline = 1;
			}
			break;
		case Ref_CMDID_RfidState:
			if (frame->Header.Len == sizeof(Ref_RfidStatus_t)) {
				memcpy(&Referee.RfidStatus, frame->Data, sizeof(Ref_RfidStatus_t));
				Referee.isOnline = 1;
			}
			break;
		case Ref_CMDID_DartClientCmd:
			if (frame->Header.Len == sizeof(Ref_DartClientCmd_t)) {
				memcpy(&Referee.DartClientCmd, frame->Data, sizeof(Ref_DartClientCmd_t));
				Referee.isOnline = 1;
			}
			break;
		case Ref_CMDID_GroundRobotPos:
			if (frame->Header.Len == sizeof(Ref_GroundRobotPos_t)) {
				memcpy(&Referee.GroundRobotPos, frame->Data, sizeof(Ref_GroundRobotPos_t));
				Referee.isOnline = 1;
			}
			break;
		case Ref_CMDID_RadarMarking:
			if (frame->Header.Len == sizeof(Ref_RadarMarking_t)) {
				memcpy(&Referee.RadarMarking, frame->Data, sizeof(Ref_RadarMarking_t));
				Referee.isOnline = 1;
			}
			break;
		case Ref_CMDID_SentryInfo:
			if (frame->Header.Len == sizeof(Ref_SentryInfo_t)) {
				memcpy(&Referee.SentryInfo, frame->Data, sizeof(Ref_SentryInfo_t));
				Referee.isOnline = 1;
			}
			break;
		case Ref_CMDID_RadarInfo:
			if (frame->Header.Len == sizeof(Ref_RadarInfo_t)) {
				memcpy(&Referee.RadarInfo, frame->Data, sizeof(Ref_RadarInfo_t));
				Referee.isOnline = 1;
			}
			break;
	}
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * @brief 注册给 UART 回调的函数
 * @note 非用户调用函数
 */
static void Referee_UART_RxCbHandler(uint8_t *Data, uint32_t Len) {
	Ref_Header_t *header;
	uint32_t FrameLen = 0;
	while (Len >= sizeof(Ref_Header_t) + 2) {
		header = (Ref_Header_t *)Data;

		if (header->SOF != Ref_SOF) return;        // 验证帧头，有误直接放弃整个buffer
		if (!Ref_CheckCRC_Header(header)) return;  // 验证CRC校验

		FrameLen = sizeof(Ref_Header_t) + header->Len + 2;

		if (!Ref_CheckCRC_Frame((Ref_t *)Data)) continue;  // 验证CRC校验
		Referee_DataParser((Ref_t *)Data);

		if (Len > FrameLen) {
			Len -= FrameLen;
			Data += FrameLen;
		} else
			return;
	}
}

static void Referee_Online_osTimer_CbHandler(void *argument) {
	Referee.isOnline = 0;
}

static void Referee_RobotStatus_osTimer_CbHandler(void *argument) {
	Referee.isOnline = 0;
	Referee.Updated_RobotStatus = 0;
	if (!Referee.RobotStatus.Robot_ID && Referee.RobotID_Last)
		Referee.RobotStatus.Robot_ID = Referee.RobotID_Last;
}

void Referee_Send(Ref_Header_t *header, uint16_t *CRC16) {
	Ref_CalcCRC_Header(header);
	Ref_CalcCRC_Frame(&header->SOF, header->Len, CRC16);
	BSP_UART_SendMessage(Referee_UART_Port, (uint8_t *)header, sizeof(Ref_Header_t) + 2 + header->Len + 2);
}

void Referee_SetExchHeader(Ref_ExchData_Header_t *header) {
	uint8_t ID = Referee.RobotStatus.Robot_ID;
	if (!ID)
		ID = Ref_RobotID_B_Infantry1;  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	header->Sender_ID = ID;
	header->Receiver_ID = Ref_GetClientFromRobot(ID);
}

void Referee_Task(void *argument) {
	osThreadExit();  // 循环内不执行任务时调用，直接退出任务
	while (1) {
		osDelay(500);
	}
}

/**
 * @brief Referee 指针
 * @return Referee_t*
 */
Referee_t *Referee_Point(void) {
	return &Referee;
}

/*********************************END OF FILE*********************************/
