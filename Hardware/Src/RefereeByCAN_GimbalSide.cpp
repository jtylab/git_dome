/**
 * @file RefereeByCAN_GimbalSide.cpp
 * @brief RoboMaster 裁判系统通信
 * @attention
 * @hardware
 * @version 
 * @date 2024-03-24
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "RefereeByCAN_GimbalSide.h"

#include "BSP_CAN.h"

#include "string.h"  // memcpy

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "timers.h"


#define RefByCAN_Port 2


__attribute__((aligned(4))) RefByCAN_t RefByCAN;

static bool ChassisOnline = 0;

static void RefByCAN_CAN_RxCbHandler(uint32_t ID, uint8_t *Data);
static void RefByCAN_osTimer_CbHandler(void *argument);

osTimerId_t RefByCAN_osTimerId;
const static osTimerAttr_t RefByCAN_osTimerAttr = {"RefByCAN_osTimer", 0, NULL, 0};
/**
 * @brief 初始化
 */
void RefByCAN_Init(void) {
	BSP_CAN_SetRxCallbackFunc(RefByCAN_Port, RefByCAN_CAN_RxCbHandler);
	RefByCAN_osTimerId = osTimerNew(RefByCAN_osTimer_CbHandler, osTimerPeriodic, NULL, &RefByCAN_osTimerAttr);  // 创建定时器
	osTimerStart(RefByCAN_osTimerId, 300);
}

bool RefByCAN_GetChassisOnline(void) {
	return ChassisOnline;
}

/**
 * @brief CAN接收回调函数
 */
void RefByCAN_CAN_RxCbHandler(uint32_t ID, uint8_t *Data) {
	/**
	 * @note 在调用xTimerResetFromISR()函数时，如果该函数导致定时器服务/守护任务离开Blocked状态（即有新的消息到达定时器命令队列），
	 *       并且该任务的优先级大于或等于当前执行任务（即被中断的任务）的优先级，则在xTimerResetFromISR()函数内部
	 *       会将pxHigherPriorityTaskWoken指向的变量设置为pdTRUE。如果xTimerResetFromISR()函数将该值设置为pdTRUE，
	 *       则在中断退出之前应进行上下文切换。
	 */
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	switch (ID) {
		case 0x500:
            memcpy(&RefByCAN.RobotStatus, Data, 8);
			xTimerResetFromISR((TimerHandle_t)RefByCAN_osTimerId, &xHigherPriorityTaskWoken);
			ChassisOnline = 1;
			break;
		case 0x501:
            memcpy(&RefByCAN.ChassisPower, Data, 8);
			xTimerResetFromISR((TimerHandle_t)RefByCAN_osTimerId, &xHigherPriorityTaskWoken);
			ChassisOnline = 1;
			break;
		case 0x502:
            memcpy(&RefByCAN.Heat, Data, 8);
			xTimerResetFromISR((TimerHandle_t)RefByCAN_osTimerId, &xHigherPriorityTaskWoken);
			ChassisOnline = 1;
			break;
		default:
			break;
	}
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void RefByCAN_osTimer_CbHandler(void *argument) {
	ChassisOnline = 0;
}

/*********************************END OF FILE*********************************/