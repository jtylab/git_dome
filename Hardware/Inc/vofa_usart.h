/**
 * @file vofa_usart.h
 * @author jiang
 * @brief vofa实时调参
 * @version 0.1
 * @date 2025-08-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef vofa_usart_H
#define vofa_usart_H

#include "BSP_UART.h"

#ifdef __cplusplus
extern "C" {
#endif

#define uart_Point 2  //串口端口号
#define One_flag  0xAA
#define Two_flag  0xBB
#define kp_flag 0x01
#define ki_flag 0x02
#define kd_flag 0x03



typedef struct PID_Data_t{
    float kp;
    float ki;
    float kd;
}PID_Data_t;



void vofa_usart_Init(void);
PID_Data_t* GetPID_DataPoint(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif