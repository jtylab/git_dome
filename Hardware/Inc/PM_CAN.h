/**
 * @file PM_CAN.h
 * @brief PowerMonitor CAN 通信
 * @attention 依赖 BSP_UART
 * @hardware 
 * @version 0.1
 * @date 2024-03-20
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __PM_CAN_H
#define __PM_CAN_H

#include "main.h"

#ifndef __cplusplus
#include "stdbool.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


#pragma pack(1)
typedef struct {
	bool Online;
    uint16_t RxData[8];
	float Voltage[4];
	float Current[4];
} PM_t;
#pragma pack()

void PM_Init(void);
PM_t *PM_Point(void);

extern float Chassis_Power;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __PM_CAN_H */

/**********************************END OF FILE**********************************/
