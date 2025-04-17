/**
 * @file TFmini.cpp
 * @brief TFmini激光测距
 * @attention
 * @hardware H-V1 H-V2
 * @version 0.1
 * @date 2024-03-04
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "TFmini.h"

#include "BSP_UART.h"

#include "string.h"  // memcpy

TFmini_t TFmini[4];

static void TFmini_UART_RxCbHandler_0(uint8_t *Data, uint32_t Len);/*
static void TFmini_UART_RxCbHandler_1(uint8_t *Data, uint32_t Len);
static void TFmini_UART_RxCbHandler_2(uint8_t *Data, uint32_t Len);
static void TFmini_UART_RxCbHandler_3(uint8_t *Data, uint32_t Len);*/

/**
 * @brief TFmini 激光测距串口初始化
 */
void TFmini_Init(void) {
	BSP_UART_SetRxCallbackFunc(1, TFmini_UART_RxCbHandler_0);
	//BSP_UART_SetRxCallbackFunc(3, TFmini_UART_RxCbHandler_1);
	//BSP_UART_SetRxCallbackFunc(4, TFmini_UART_RxCbHandler_2);
	//BSP_UART_SetRxCallbackFunc(5, TFmini_UART_RxCbHandler_3);
}

/**
 * @brief TFmini 数据解包
 */
static void TFmini_DataHandler(TFmini_t * TFmini, uint8_t *Data, uint32_t Len) {
	uint8_t Sum = 0;
	if (Len < 9) return;
	for (int i = 0; i < 8; i++) {
		Sum += Data[i];
	}
	if (Sum == Data[9]) {
		memcpy(&TFmini->Head[0], Data, 9);
		TFmini->Temp = TFmini->TempRaw / 8 - 256;
	}
}

/**
 * @brief 注册给 UART 的回调函数
 * @note 非用户调用函数
 */
static void TFmini_UART_RxCbHandler_0(uint8_t *Data, uint32_t Len) {
	TFmini_DataHandler(&TFmini[0], Data, Len);
}
/*
static void TFmini_UART_RxCbHandler_1(uint8_t *Data, uint32_t Len) {
	TFmini_DataHandler(&TFmini[1], Data, Len);
}

static void TFmini_UART_RxCbHandler_2(uint8_t *Data, uint32_t Len) {
	TFmini_DataHandler(&TFmini[2], Data, Len);
}

static void TFmini_UART_RxCbHandler_3(uint8_t *Data, uint32_t Len) {
	TFmini_DataHandler(&TFmini[3], Data, Len);
}
*/
/**
 * @brief TFmini 激光测距指针
 */
TFmini_t *TFmini_Point(uint8_t Code) {
	return &TFmini[Code];
}

/*********************************END OF FILE*********************************/
