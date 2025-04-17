/**
 * @file RM_VTM.cpp
 * @brief RM 图传遥控控制链路通信
 * @attention
 * @hardware H-V1 H-V2
 * @version 0.1
 * @date 2024-03-04
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "RM_VTM.h"

#include "Referee_Protocol.h"

#include "BSP_UART.h"

#include "string.h"  // memcpy

#include "cmsis_os.h"
#include "timers.h"

#define VTM_UART_Port 1

#define K_MoveSpeed_Fast   660.0f
#define K_MoveSpeed_Normal 330.0f
#define K_MoveSpeed_Slow   110.0f

static VTM_t VTM = {0};
static void VTM_UART_RxCbHandler(uint8_t *Data, uint32_t Len);
static void VTM_osTimer_CbHandler(void *argument);

osTimerId_t VTM_osTimerId;
const static osTimerAttr_t VTM_osTimerAttr = {"VTM_osTimer", 0, NULL, 0};
/**
 * @brief VTM 初始化
 */
void VTM_Init(void) {
	BSP_UART_SetRxCallbackFunc(VTM_UART_Port, VTM_UART_RxCbHandler);
	VTM_osTimerId = osTimerNew(VTM_osTimer_CbHandler, osTimerPeriodic, NULL, &VTM_osTimerAttr);
	osTimerStart(VTM_osTimerId, 100);  // 100ms
}

/**
 * @brief 图传数据处理
 * @note 非用户调用函数
 */
static void VTM_DataParser(Ref_t *frame) {
	/**
	 * @note 在调用xTimerResetFromISR()函数时，如果该函数导致定时器服务/守护任务离开Blocked状态（即有新的消息到达定时器命令队列），
	 *       并且该任务的优先级大于或等于当前执行任务（即被中断的任务）的优先级，则在xTimerResetFromISR()函数内部
	 *       会将pxHigherPriorityTaskWoken指向的变量设置为pdTRUE。如果xTimerResetFromISR()函数将该值设置为pdTRUE，
	 *       则在中断退出之前应进行上下文切换。
	 */
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if (frame->CMD_ID != Ref_CMDID_VTM_KbdData) return;
	if (frame->Header.Len != sizeof(VTM_Raw_t)) return;

	xTimerResetFromISR((TimerHandle_t)VTM_osTimerId, &xHigherPriorityTaskWoken);

	memcpy(&VTM.Raw, frame->Data, sizeof(VTM_Raw_t));
	VTM.isOnline = 1;

	if (VTM.Raw.KeyBoard.Key.SHIFT) {
		VTM.MoveX = -K_MoveSpeed_Fast * VTM.Raw.KeyBoard.Key.A + K_MoveSpeed_Fast * VTM.Raw.KeyBoard.Key.D;
		VTM.MoveY = -K_MoveSpeed_Fast * VTM.Raw.KeyBoard.Key.S + K_MoveSpeed_Fast * VTM.Raw.KeyBoard.Key.W;
	} else if (VTM.Raw.KeyBoard.Key.CTRL) {
		VTM.MoveX = -K_MoveSpeed_Slow * VTM.Raw.KeyBoard.Key.A + K_MoveSpeed_Slow * VTM.Raw.KeyBoard.Key.D;
		VTM.MoveY = -K_MoveSpeed_Slow * VTM.Raw.KeyBoard.Key.S + K_MoveSpeed_Slow * VTM.Raw.KeyBoard.Key.W;
	} else {
		VTM.MoveX = -K_MoveSpeed_Normal * VTM.Raw.KeyBoard.Key.A + K_MoveSpeed_Normal * VTM.Raw.KeyBoard.Key.D;
		VTM.MoveY = -K_MoveSpeed_Normal * VTM.Raw.KeyBoard.Key.S + K_MoveSpeed_Normal * VTM.Raw.KeyBoard.Key.W;
	}

	if (VTM.Raw.KeyBoard.Key.SHIFT) {
		VTM.Yaw = VTM.Raw.Mouse.X;
		VTM.Pitch = -VTM.Raw.Mouse.Y;
	} else {
		VTM.Yaw = 0.5f * VTM.Raw.Mouse.X;
		VTM.Pitch = 0.5f * (-VTM.Raw.Mouse.Y);
	}

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * @brief 注册给 UART 回调的函数
 * @note 非用户调用函数
 */
static void VTM_UART_RxCbHandler(uint8_t *Data, uint32_t Len) {
	Ref_Header_t *header = (Ref_Header_t *)Data;
	uint32_t Offset = 0, FrameLen = 0;
	while (Len >= sizeof(Ref_Header_t) + 2) {
		header = (Ref_Header_t *)(Data + Offset);

		if (header->SOF != Ref_SOF) return;        // 验证帧头，有误直接放弃整个buffer
		if (!Ref_CheckCRC_Header(header)) return;  // 验证CRC校验

		FrameLen = sizeof(Ref_Header_t) + header->Len + 2;
		Offset += FrameLen;

		if (!Ref_CheckCRC_Frame((Ref_t *)Data)) continue;  // 验证CRC校验
		VTM_DataParser((Ref_t *)Data);

		if (Len > FrameLen)
			Len -= FrameLen;
		else
			return;
	}
}

static void VTM_osTimer_CbHandler(void *argument) {
	VTM.MoveX = 0;
	VTM.MoveY = 0;
	VTM.Yaw = 0;
	VTM.Pitch = 0;
	VTM.isOnline = 0;
}

/**
 * @brief 返回图传指针
 * @return VTM_t * 图传指针
 */
VTM_t *VTM_Point(void) {
	return &VTM;
}

/*********************************END OF FILE*********************************/
