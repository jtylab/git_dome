/**
 * @file BSP_UART.h
 * @brief 板级 UART 支持包
 * @attention 需要在 main 函数中进行初始化
 * @hardware 
 * @version 0.1
 * @date 2023-07-09
 * @author tianxu, he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "main.h"

#include "stdio.h"  // sprintf
#include "string.h"  // strlen

/**
 * @ingroup BSP_UART
 * @defgroup BSP_UART BSP_UART
 * @brief 板级 UART 支持包
 * @{
 */

#define BSP_UART_printf(Port,Format,...){ \
	if(1){ \
		char TempString[64]; \
		sprintf(TempString,Format,##__VA_ARGS__); \
		BSP_UART_SendMessage((Port),(unsigned char *)(TempString),strlen(TempString)); \
	} \
}

#define BSP_UART_printf_Block(Port,Format,...){	\
	if(1){ \
		char TempString[64]; \
		sprintf(TempString,Format,##__VA_ARGS__); \
		BSP_UART_SendMessage_Block((Port),(unsigned char *)(TempString),strlen(TempString)); \
	} \
}

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*BSP_UART_CallbackFunc_t)(uint8_t *, uint32_t);  // 参数为数据缓冲区，数据长度

void BSP_UART_Init(void);
uint8_t BSP_UART_SetBaudrate(uint8_t Port, uint32_t baud);
uint8_t BSP_UART_SendMessage(uint8_t Port, uint8_t *Data, uint32_t Len);
uint8_t BSP_UART_SendMessage_Block(uint8_t Port, uint8_t *Data, uint32_t Len);
uint8_t BSP_UART_SendMessage_DMA(uint8_t Port, uint8_t *Data, uint32_t Len);
uint8_t BSP_UART_SetRxCallbackFunc(uint8_t Port, BSP_UART_CallbackFunc_t Function);

void UART_USB_RecvHandler(uint8_t *Buf, uint32_t Len);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __BSP_UART_H */

/**
 * @}
 */

/*********************************END OF FILE*********************************/
