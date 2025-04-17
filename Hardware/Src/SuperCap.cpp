/**
 * @file SuperCap.cpp
 * @brief 安合超级电容控制板
 * @attention
 * @hardware 
 * @version 0.1
 * @date 2024-03-24
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "SuperCap.h"

#include "BSP_CAN.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"

#define SuperCap_CAN_Port 2
#define SuperCap_ReadOnly 1

SuperCap_t Super(SuperCap_CAN_Port);

void SuperCap_CAN_RxCbHandler(uint32_t ID, uint8_t *Data);
void SuperCapTask(void *argument);

osThreadId_t SuperCapHandle;
static const osThreadAttr_t SuperCapTask_attributes = {"SuperCapTask", 0, 0, 0, 0, 256, (osPriority_t)osPriorityNormal5};
/**
 * @brief 超级电容初始化
 */
void SuperCap_Init(void) {
	Super.Init();
	BSP_CAN_SetRxCallbackFunc(SuperCap_CAN_Port, SuperCap_CAN_RxCbHandler);                          // 向BSP_CAN申请回调函数
#ifndef SuperCap_ReadOnly
	SuperCapHandle = osThreadNew(SuperCapTask, NULL, &SuperCapTask_attributes);  // 创建超级电容线程
#endif
}

/**
 * @brief 超级电容类初始化
 */
void SuperCap_t::Init(void) {
	isInit = 1;
}

uint8_t SuperCap_t::GetPort(void) {
	return Prv_CAN_Port;
}

/**
 * @brief 设置超级电容运行状态/是否保存当前状态
 * @param State 状态 0停机 1运行（输出负载开关断开） 2运行（输出负载开关打开）
 * @param save 是否保存为上电默认
 */
void SuperCap_t::SetInputState(uint16_t State, bool save) {
	Set_Input_State = State;
	Set_Input_State_Save = save;
}

/**
 * @brief 设置超级电容功率上限/是否保存当前功率上限
 * @param Power 功率上限 单位0.01W 默认8000
 * @param save 是否保存为上电默认
 */
void SuperCap_t::SetInputPower(uint16_t Power, bool save) {
	Set_Input_Power = Power;
	Set_Input_Power_Save = save;
}

/**
 * @brief 设置超级电容输出电压/是否保存当前输出电压
 * @param Voltage 输出电压 单位0.01V 默认2400
 * @param save 是否保存为上电默认
 */
void SuperCap_t::SetInputVoltage(uint16_t Voltage, bool save) {
	Set_Input_Voltage = Voltage;
	Set_Input_Voltage_Save = save;
}

/**
 * @brief 设置超级电容输出电流/是否保存当前输出电流
 * @param Current 输出电流 单位0.01A 默认800
 * @param save 是否保存为上电默认
 */
void SuperCap_t::SetInputCurrent(uint16_t Current, bool save) {
	Set_Input_Current = Current;
	Set_Input_Current_Save = save;
}

/**
 * @brief CAN 发送超级电容输出值
 * @param Port CAN 编号
 * @param ID 超级电容使用的标识符
 * @param arg 参数
 * @param save 是否保存为上电默认
 */
static void CAN_SendToSuperCap(uint8_t Port, uint32_t ID, uint16_t arg, uint16_t save) {
	uint8_t TxData[8];
	TxData[0] = (arg >> 8) & 0xFF;
	TxData[1] = arg & 0xFF;
	TxData[2] = (save >> 8) & 0xFF;
	TxData[3] = save & 0xFF;
	TxData[4] = 0;
	TxData[5] = 0;
	TxData[6] = 0;
	TxData[7] = 0;
	BSP_CAN_SendStandard8Data(Port, ID, TxData);
}

/**
 * @brief 发送超级电容状态
 * @param ID 超级电容使用的标识符
 */
static void SuperCap_SendState(uint8_t Port) {
	if (!Super.isInit) return;
	CAN_SendToSuperCap(Port, 0x600, Super.Set_Input_State, Super.Set_Input_State_Save);
}

/**
 * @brief 发送超级电容所有数据
 * @param ID 超级电容使用的标识符
 */
static void SuperCap_SendAll(uint8_t Port) {
	uint8_t tmp = 0;
	if (!Super.isInit) return;
	CAN_SendToSuperCap(Port, 0x600, Super.Set_Input_State, Super.Set_Input_State_Save);
	osDelay(1);
	CAN_SendToSuperCap(Port, 0x601, Super.Set_Input_Power, Super.Set_Input_Power_Save);
	osDelay(1);
	CAN_SendToSuperCap(Port, 0x602, Super.Set_Input_Voltage, Super.Set_Input_Voltage_Save);
	osDelay(1);
	CAN_SendToSuperCap(Port, 0x603, Super.Set_Input_Current, Super.Set_Input_Current_Save);
	osDelay(1);
	BSP_CAN_SendRemoteData(Port, 0x610, &tmp, 1);  // 用远程帧发送数据才会回传数据
	osDelay(1);
	BSP_CAN_SendRemoteData(Port, 0x611, &tmp, 1);
	osDelay(1);
	BSP_CAN_SendRemoteData(Port, 0x612, &tmp, 1);
	osDelay(1);
	BSP_CAN_SendRemoteData(Port, 0x613, &tmp, 1);
}

/**
 * @brief 超级电容线程函数
 */
void SuperCapTask(void *argument) {
	SuperCap_SendState(Super.GetPort());
	osDelay(100);
	// osDelay(3500);
	while (1) {
		SuperCap_SendAll(Super.GetPort());
		osDelay(1);
	}
}

/**
 * @brief 超级电容 CAN 中断回调
 */
void SuperCap_CAN_RxCbHandler(uint32_t ID, uint8_t *Data) {
	switch (ID) {  // clang-format off
		case 0x600:
			Super.Input_State =        (Data[0] << 8) + Data[1];
			Super.Input_State_Save =   (Data[2] << 8) + Data[3];
			break;
		case 0x601:
			Super.Input_Power =        (Data[0] << 8) + Data[1];
			Super.Input_Power_Save =   (Data[2] << 8) + Data[3];
			break;
		case 0x602:
			Super.Input_Voltage =      (Data[0] << 8) + Data[1];
			Super.Input_Voltage_Save = (Data[2] << 8) + Data[3];
			break;
		case 0x603:
			Super.Input_Current =      (Data[0] << 8) + Data[1];
			Super.Input_Current_Save = (Data[2] << 8) + Data[3];
			break;
		case 0x610:
			Super.Output_State =       (Data[0] << 8) + Data[1];
			Super.Output_Fault =       (Data[2] << 8) + Data[3];
			break;
		case 0x611:
			Super.Output_InPower =     (Data[0] << 8) + Data[1];
			Super.Output_InVoltage =   (Data[2] << 8) + Data[3];
			Super.Output_InCurrent =   (Data[4] << 8) + Data[5];
			break;
		case 0x612:
			Super.Output_OutPower =    (Data[0] << 8) + Data[1];
			Super.Output_OutVoltage =  (Data[2] << 8) + Data[3];
			Super.Output_OutCurrent =  (Data[4] << 8) + Data[5];
			break;
		case 0x613:
			Super.Output_Temp =        (Data[0] << 8) + Data[1];
			Super.Output_AddTime =     (Data[2] << 8) + Data[3];
			Super.Output_ThisTime =    (Data[4] << 8) + Data[5];
			break;  // clang-format on
	}
}

/**
 * @brief 超级电容数据指针
 * @return SuperCap_t* 指针
 */
SuperCap_t *Super_Point(void) {
	return &Super;
}

/*********************************END OF FILE*********************************/
