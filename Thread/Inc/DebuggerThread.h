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
 */
#ifndef __DebuggerThread_H
#define __DebuggerThread_H

#include "main.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"

#include "BSP_UART.h"
#include "Robodefine.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)
typedef struct {
	uint8_t Head[2];
	uint8_t Type;
	uint32_t Code;
	double Data;
	uint8_t Sum;
} Debugger_t;
#pragma pack()

extern double Debugger[17 * 22];
extern double DebuggerLine[10];
void DebuggerThread_Init(void);
uint8_t Debugger_IsConnect(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __DebuggerThread_H */

/*********************************END OF FILE*********************************/
