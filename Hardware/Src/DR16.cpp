/**
 * @file DR16.cpp
 * @brief DR16 接收机
 * @attention 依赖 BSP_UART
 * @hardware 
 * @version 0.1
 * @date 2022-03-13 (updated 2024-03-22 (by he))
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "DR16.h"

#include "BSP_UART.h"

#include "string.h"  // memset

#define DR16_UART_Port 3  // 3:DJI-C

DR16_t DR16;

bool DR16_OnlineFlag = 0;
static void DR16_UART_RxCbHandler(uint8_t *Data, uint32_t Len);

/**
 * @brief 遥控器初始化
 */
void DR16_Init(void) {
	BSP_UART_SetRxCallbackFunc(DR16_UART_Port, DR16_UART_RxCbHandler);
}

/**
 * @brief 串口数据回调 遥控器解码
 */
static void DR16_UART_RxCbHandler(uint8_t *Data, uint32_t Len) {
	if (Len != 18) {
		DR16_OnlineFlag = 0;
		return;
	}
	DR16.CH1 = (Data[0] | Data[1] << 8) & 0x07FF;
	DR16.CH1 -= 1024;
	DR16.CH2 = (Data[1] >> 3 | Data[2] << 5) & 0x07FF;
	DR16.CH2 -= 1024;
	DR16.CH3 = (Data[2] >> 6 | Data[3] << 2 | Data[4] << 10) & 0x07FF;
	DR16.CH3 -= 1024;
	DR16.CH4 = (Data[4] >> 1 | Data[5] << 7) & 0x07FF;
	DR16.CH4 -= 1024;

	DR16.SW1 = ((Data[5] >> 4) & 0x000C) >> 2;
	DR16.SW2 = (Data[5] >> 4) & 0x0003;

	DR16.Mouse.X = Data[6] | (Data[7] << 8);
	DR16.Mouse.Y = Data[8] | (Data[9] << 8);
	DR16.Mouse.Z = Data[10] | (Data[11] << 8);

	DR16.Mouse.L = Data[12];
	DR16.Mouse.R = Data[13];

	DR16.KeyBoard.KeyCode = Data[14] | Data[15] << 8;
	DR16.Wheel = (Data[16] | Data[17] << 8) - 1024;

	DR16.Online++;
	if (DR16.Online > 60) {
		DR16.Online = 60;
		DR16_OnlineFlag = 1;
	}
	if ((DR16.CH1 > 660 || DR16.CH1 < -660) ||
		(DR16.CH2 > 660 || DR16.CH2 < -660) ||
		(DR16.CH3 > 660 || DR16.CH3 < -660) ||
		(DR16.CH4 > 660 || DR16.CH4 < -660)) {
		memset(&DR16, 0, sizeof(DR16_t));
	}
}

/**
 * @brief 返回遥控器指针
 * @return DR16_t 遥控器指针
 */
DR16_t *DR16_Point(void) {
	return &DR16;
}

/*********************************END OF FILE*********************************/
