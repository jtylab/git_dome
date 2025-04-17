/**
 * @file PM_CAN.cpp
 * @brief PowerMonitor CAN 通信
 * @attention 依赖 BSP_CAN
 * @hardware
 * @version 0.1
 * @date 2024-03-20
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "PM_CAN.h"

#include "BSP_CAN.h"

#include "string.h"  // memset

#include "LPF.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "timers.h"

#define PM_CAN_Port 1

static PM_t PM = {0};

float Chassis_Power = 0.0f;
LPF_t LPF_Chassis_Power;

const float ADC_Vref = 3.28334f;           // Vref (Volt)
const float ADC_VrefH = ADC_Vref * 0.5f;   // half Vref (Volt)
const float ADC_LSB = ADC_Vref / 4095.0f;  // 1LSB (Volt)
const float ADC_Vrefint = 1.21712f;        // Vref (Volt)

osTimerId_t PM_osTimerId;
const static osTimerAttr_t PM_osTimerAttr = {"PM_osTimer", 0, NULL, 0};
static void PM_osTimer_CbHandler(void *argument);
static void PM_CAN_RxCbHandler(uint32_t ID, uint8_t *Data);

/**
 * @brief 初始化
 */
void PM_Init(void) {
	BSP_CAN_SetRxCallbackFunc(PM_CAN_Port, PM_CAN_RxCbHandler);
	PM_osTimerId = osTimerNew(PM_osTimer_CbHandler, osTimerPeriodic, NULL, &PM_osTimerAttr);
	osTimerStart(PM_osTimerId, 10);  // 1ms
	LPF_Chassis_Power.Init(0.2, 5);
}

/**
 * @brief 串口数据回调 数据解码
 */
static void PM_CAN_RxCbHandler(uint32_t ID, uint8_t *Data) {
	/**
	 * @note 在调用xTimerResetFromISR()函数时，如果该函数导致定时器服务/守护任务离开Blocked状态（即有新的消息到达定时器命令队列），
	 *       并且该任务的优先级大于或等于当前执行任务（即被中断的任务）的优先级，则在xTimerResetFromISR()函数内部
	 *       会将pxHigherPriorityTaskWoken指向的变量设置为pdTRUE。如果xTimerResetFromISR()函数将该值设置为pdTRUE，
	 *       则在中断退出之前应进行上下文切换。
	 */
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	switch (ID) {
		case 0x0C1:
			memcpy(&PM.RxData[0], Data, 8);
			PM.Current[0] = 10.0f * ((float)(PM.RxData[0]) * ADC_LSB - ADC_VrefH);  // 计算电流 (A)
			PM.Current[1] = 10.0f * ((float)(PM.RxData[1]) * ADC_LSB - ADC_VrefH);  // 计算电流 (A)
			PM.Current[2] = 10.0f * ((float)(PM.RxData[2]) * ADC_LSB - ADC_VrefH);  // 计算电流 (A)
			PM.Current[3] = 10.0f * ((float)(PM.RxData[3]) * ADC_LSB - ADC_VrefH);  // 计算电流 (A)
			xTimerResetFromISR((TimerHandle_t)PM_osTimerId, &xHigherPriorityTaskWoken);
			PM.Online = 1;
			break;
		case 0x0C2:
			memcpy(&PM.RxData[4], Data, 8);
			PM.Voltage[0] = 11.0f * (float)(PM.RxData[4]) * ADC_LSB;  // 计算电压 (V)
			PM.Voltage[1] = 11.0f * (float)(PM.RxData[5]) * ADC_LSB;  // 计算电压 (V)
			PM.Voltage[2] = 11.0f * (float)(PM.RxData[6]) * ADC_LSB;  // 计算电压 (V)
			PM.Voltage[3] = 11.0f * (float)(PM.RxData[7]) * ADC_LSB;  // 计算电压 (V)
			xTimerResetFromISR((TimerHandle_t)PM_osTimerId, &xHigherPriorityTaskWoken);
			PM.Online = 1;
			break;
		default:
			break;
	}
	Chassis_Power = LPF_Chassis_Power.Generate(PM.Voltage[1] * PM.Current[1]);
}

void PM_osTimer_CbHandler(void *argument) {
	PM.Online = 0;
}

/**
 * @brief 返回指针
 * @return PM_t 指针
 */
PM_t *PM_Point(void) {
	return &PM;
}

/**********************************END OF FILE**********************************/
