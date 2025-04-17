/**
 * @file DebuggerThread.cpp
 * @brief 调参器线程 v4.0
 * @attention 依赖 BSP_UART；上位机软件在'../Assistance/'目录中
 * @hardware H-V1 H-V2
 * @version 0.1
 * @date 2021-09-26
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 * @note
 *   DebuggerLine[0] -> 绿色
 *   DebuggerLine[1] -> 蓝色
 *   DebuggerLine[2] -> 红色
 *   DebuggerLine[3] -> 黄色
 *   DebuggerLine[4] -> 青蓝
 *   DebuggerLine[5] -> 品红
 *   DebuggerLine[6] -> 黑色
 *   DebuggerLine[7] -> 紫色
 *   DebuggerLine[8] -> 灰色
 *   DebuggerLine[9] -> 墨绿
 *
 */
#include "DebuggerThread.h"



#define UART 10  // 进行调参所用的串口编号，详见BSP_UART.cpp

double Debugger[17 * 22] = {0};  // 在其他线程更改该数组，参数自动回传到上位机，更改上位机，参数自动下传到该数组
double DebuggerLine[10] = {0};   // 在其他线程更改该数组，自动在上位机画曲线轨迹
double Debugger_Last[17 * 22] = {0};
double DebuggerLine_Last[10] = {0};

static uint8_t Debugger_ConnectFlag = 0;
static uint8_t Debugger_Connect = 0;
static uint8_t Debugger_Check = 0;

static Debugger_t TxData = {0};
static Debugger_t RxData = {0};

static void UART_RxCallBack(uint8_t *Data, uint32_t Len);

void DebuggerDecode(void);
void DebuggerTask(void *argument);

osThreadId_t DebuggerThreadHandle;
static const osThreadAttr_t DebuggerTask_attributes = {"DebuggerTask", 0, 0, 0, 0, 256, (osPriority_t)osPriorityLow2};
/**
 * @brief 调参器串口、线程初始化函数
 * @call main.c
 */
void DebuggerThread_Init(void) {
	TxData.Head[0] = 0xAC;
	TxData.Head[1] = 0xCA;
	BSP_UART_SetRxCallbackFunc(UART, UART_RxCallBack);
	DebuggerThreadHandle = osThreadNew(DebuggerTask, NULL, &DebuggerTask_attributes);  // 创建线程
}

/**
 * @brief 调参器串口发送函数
 * @param Type 包类型
 * @param Code
 * @param Data 数据
 */
void Debugger_Send(uint8_t Type, uint32_t Code, double Data) {
	uint8_t Sum = 0;
	TxData.Type = Type;
	TxData.Code = Code;
	TxData.Data = Data;
	TxData.Sum = 0;
	for (int i = 0; i < sizeof(TxData); i++) {
		Sum += *((uint8_t *)(&TxData) + i);
	}
	TxData.Sum = Sum;
	BSP_UART_SendMessage(UART, ((uint8_t *)(&TxData)), sizeof(TxData));
	osDelay(5);
}

/**
 * @brief 调参器串口中断回调
 */
static void UART_RxCallBack(uint8_t *Data, uint32_t Len) {
	uint8_t Sum;
	if (Len != sizeof(RxData)) return;
	RxData = *((Debugger_t *)(Data));
	Sum = 0;
	for (int i = 0; i < sizeof(RxData) - 1; i++) {
		Sum += *((uint8_t *)(&RxData) + i);
	}
	if (RxData.Sum == Sum && RxData.Head[0] == 0xAB && RxData.Head[1] == 0xBA) {
		Debugger_Connect = 1;  // 更新连接状态
		Debugger_Check = 1;    // 反馈成功包
		if (RxData.Type == 1) {
			Debugger[RxData.Code] = RxData.Data;
			Debugger_Last[RxData.Code] = RxData.Data;
		}
	}
}

/**
 * @brief 调参器线程
 * @call freertos.c
 */
void DebuggerTask(void *argument) {
	int ConnectTimer = 0;
	osDelay(3500);
	Debugger_Send(255, 0, 0);
	while (1) {
		for (int i = 0; i < 10; i++) {
			if (DebuggerLine[i] != DebuggerLine_Last[i]) {
				DebuggerLine_Last[i] = DebuggerLine[i];
				Debugger_Send(2, i, DebuggerLine[i]);
			}
		}
		for (int i = 0; i < 17 * 22; i++) {
			if (Debugger[i] != Debugger_Last[i]) {
				Debugger_Last[i] = Debugger[i];
				Debugger_Send(1, i, Debugger[i]);
			}
		}
		if (Debugger_Check) {
			Debugger_Check = 0;
			Debugger_Send(0, 0, 0);  // 成功反馈包
		}
		osDelay(10);
		ConnectTimer += 10;
		if (ConnectTimer == 500) {
			Debugger_ConnectFlag = Debugger_Connect;
			Debugger_Connect = 0;
			ConnectTimer = 0;
		}
	}
}

/**
 * @brief 调参器是否连接
 * @return uint8_t
 */
uint8_t Debugger_IsConnect(void) {
	return Debugger_ConnectFlag;
}

/*********************************END OF FILE*********************************/
