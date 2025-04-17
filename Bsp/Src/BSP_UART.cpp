/**
 * @file BSP_UART.cpp
 * @brief 板级 UART 支持包
 * @attention 需要在 main 函数中进行初始化，
 * @hardware
 * @version 0.1
 * @date 2023-07-09
 * @author tianxu, he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "BSP_UART.h"

#include "usart.h"
#include "usbd_cdc_if.h"


#define UART_NUM       3
#define USB_CDC_NUM    1
#define TOTAL_UART_NUM (UART_NUM + USB_CDC_NUM)
#define BUFFSIZE       512


static UART_HandleTypeDef *UART_Handle[UART_NUM] = {&huart6, &huart1, &huart3};
static BSP_UART_CallbackFunc_t CB_Func[TOTAL_UART_NUM] = {0};

__attribute__((aligned(4))) uint8_t UART_Buff[UART_NUM][BUFFSIZE];
__attribute__((aligned(4))) uint8_t UART_Buff_USB[BUFFSIZE];

/**
 * @brief 初始化 UART
 * @note 需要在 main 中调用
 */
void BSP_UART_Init(void) {
	for (int i = 0; i < UART_NUM; i++) {
		while (HAL_UARTEx_ReceiveToIdle_DMA(UART_Handle[i], &UART_Buff[i][0], BUFFSIZE) != HAL_OK) {
			__HAL_UNLOCK(UART_Handle[i]);
		}
	}
}

/**
 * @brief 根据 Port 号获取句柄
 * @param Port 串口号，范围（1~UART_NUM）
 * @return UART_HandleTypeDef 句柄
 */
UART_HandleTypeDef *BSP_UART_GetHandler(uint8_t Port) {
	if (Port < 1) {
		return NULL;
	} else if (Port <= UART_NUM) {
		return UART_Handle[Port - 1];
	} else {
		return NULL;
	}
}

/**
 * @brief UART 设置波特率
 * @param Port 串口号，范围（1~UART_NUM）
 * @param baud 波特率
 * @return uint8_t 状态
 * @retval 0 成功
 * @retval 1 失败
 */
uint8_t BSP_UART_SetBaudrate(uint8_t Port, uint32_t baud) {
	UART_HandleTypeDef *pHandler = BSP_UART_GetHandler(Port);
	if (!pHandler) return 1;
	HAL_UART_DMAStop(pHandler);
	HAL_UART_AbortReceive_IT(pHandler);
	pHandler->Init.BaudRate = baud;
	HAL_UART_DeInit(pHandler);
	// if (HAL_UART_Init(pHandler) != HAL_OK) return 1;
	HAL_UART_Init(pHandler);
	for (int i = 0; i < UART_NUM; i++) {
		if (pHandler != UART_Handle[i]) continue;
		while (HAL_UARTEx_ReceiveToIdle_DMA(UART_Handle[i], &UART_Buff[i][0], BUFFSIZE) != HAL_OK) {
			__HAL_UNLOCK(UART_Handle[i]);
		}
	}
	return 0;
}

uint32_t waiting_count = 0;
uint32_t repair_count = 0;
void _Repair_SetStateReady(UART_HandleTypeDef *uart) {
	waiting_count++;
	if (waiting_count > 100) {
		uart->gState = HAL_UART_STATE_READY;
		waiting_count = 0;
		repair_count++;
	}
}

/**
 * @brief UART 发送数据
 * @param Port 串口号:范围（1~UART_NUM）
 * @param Data 待发送的8字节数据
 * @param Len 待发送数据长度
 * @return uint8_t 状态
 * @retval 0 成功
 * @retval 1 失败
 */
uint8_t BSP_UART_SendMessage(uint8_t Port, uint8_t *Data, uint32_t Len) {
	if (Port == TOTAL_UART_NUM) return !(CDC_Transmit_FS(Data, Len) == USBD_OK);
	UART_HandleTypeDef *pHandler = BSP_UART_GetHandler(Port);
	if (!pHandler) return 1;
	while (HAL_UART_Transmit_IT(pHandler, Data, Len) != HAL_OK) {
		__HAL_UNLOCK(pHandler);
		_Repair_SetStateReady(pHandler);
	}
	return 0;
}

/**
 * @brief UART 发送数据（阻塞）
 * @param Port 串口号，范围（1~UART_NUM）
 * @param Data 待发送的8字节数据指针
 * @param Len 待发送数据长度
 * @return uint8_t 状态
 * @retval 0 成功
 * @retval 1 失败
 */
uint8_t BSP_UART_SendMessage_Block(uint8_t Port, uint8_t *Data, uint32_t Len) {
	if (Port == TOTAL_UART_NUM) return !(CDC_Transmit_FS(Data, Len) == USBD_OK);
	UART_HandleTypeDef *pHandler = BSP_UART_GetHandler(Port);
	if (!pHandler) return 1;
	while (HAL_UART_Transmit_IT(pHandler, Data, Len) != HAL_OK) {
		__HAL_UNLOCK(pHandler);
		_Repair_SetStateReady(pHandler);
	}
	return 0;
}

/**
 * @brief UART 发送数据 (DMA)
 * @param Port 串口号，范围（1~UART_NUM）
 * @param Data 待发送的8字节数据指针
 * @param Len 待发送数据长度
 * @return uint8_t 状态
 * @retval 0 成功
 * @retval 1 失败
 */
uint8_t BSP_UART_SendMessage_DMA(uint8_t Port, uint8_t *Data, uint32_t Len) {
	if (Port == TOTAL_UART_NUM) return !(CDC_Transmit_FS(Data, Len) == USBD_OK);
	UART_HandleTypeDef *pHandler = BSP_UART_GetHandler(Port);
	if (!pHandler) return 1;
	while (HAL_UART_Transmit(pHandler, Data, Len, 100) != HAL_OK) {
		__HAL_UNLOCK(pHandler);
		_Repair_SetStateReady(pHandler);
	}
	return 0;
}

/**
 * @brief 设置 UART 接收回调函数
 * @param Port 串口号
 * @param Function 函数指针 其中，函数的结构为 void UART_RxCallBack(uint8_t *Data, uint32_t Len)
 * @return uint8_t 状态
 * @retval 0 成功
 * @retval 1 失败
 */
uint8_t BSP_UART_SetRxCallbackFunc(uint8_t Port, BSP_UART_CallbackFunc_t Function) {
	UART_HandleTypeDef *pHandler;
	if (Port != TOTAL_UART_NUM) {
		pHandler = BSP_UART_GetHandler(Port);
		if (!pHandler) return 1;
	}
	if (CB_Func[Port - 1]) return 1;
	CB_Func[Port - 1] = Function;
	return 0;
}

/**
 * @brief UART 接收事件回调函数
 * @note 由 HAL 调用，上层无需关心
 * @param huart
 * @param Size 数据长度
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	for (int i = 0; i < UART_NUM; i++) {
		if (huart == UART_Handle[i]) {
			HAL_UART_DMAStop(huart);
			if (CB_Func[i] != NULL) CB_Func[i](&UART_Buff[i][0], Size);
			while (HAL_UARTEx_ReceiveToIdle_DMA(huart, &UART_Buff[i][0], BUFFSIZE) != HAL_OK) {
				__HAL_UNLOCK(huart);
			}
			return;
		}
	}
}

/**
 * @brief USB CDC VPC 接收回调函数
 * @note 移植 BSP 时，在 USB_Device 调用，上层无需关心
 * @param Buf
 * @param Len 数据长度
 */
void UART_USB_RecvHandler(uint8_t *Buf, uint32_t Len) {
	memcpy(UART_Buff_USB, Buf, Len);
	if (CB_Func[TOTAL_UART_NUM - 1] != NULL) CB_Func[TOTAL_UART_NUM - 1](UART_Buff_USB, Len);
}


struct {
	uint16_t Error;
	uint16_t ParityError;
	uint16_t FrameError;
	uint16_t NoiseError;
	uint16_t DMAError;
} UART_ErrorCount[UART_NUM] = {0};

/**
 * @brief UART 错误回调函数
 * @note 由 HAL 调用，上层无需关心
 * @param huart
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	uint32_t errorcode;
	for (int i = 0; i < UART_NUM; i++) {
		if (huart == UART_Handle[i]) {
			UART_ErrorCount[i].Error++;
			errorcode = huart->ErrorCode;
			if (errorcode & HAL_UART_ERROR_PE) {
				UART_ErrorCount[i].ParityError++;
			}
			if (errorcode & HAL_UART_ERROR_FE) {
				UART_ErrorCount[i].FrameError++;
			}
			if (errorcode & HAL_UART_ERROR_NE) {
				UART_ErrorCount[i].NoiseError++;
			}
			if (errorcode & HAL_UART_ERROR_DMA) {
				UART_ErrorCount[i].DMAError++;
				HAL_UART_DMAStop(huart);
				while (HAL_UARTEx_ReceiveToIdle_DMA(huart, &UART_Buff[i][0], BUFFSIZE) != HAL_OK) {
					__HAL_UNLOCK(huart);
				}
			}
			return;
		}
	}
}

/*********************************END OF FILE*********************************/
