/**
 * @file K230_Uart.h
 * @author jiang
 * @brief 视觉通信
 * @version 0.1
 * @date 2025-07-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #ifndef K230_Uart_H
 #define K230_Uart_H

 
#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"



void K230_UartTask_Init(void);
 
 #endif // !