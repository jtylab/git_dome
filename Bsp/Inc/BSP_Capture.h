/**
 * @file BSP_Capture.h
 * @brief 板级Capture支持包
 * @attention
 * @hardware H-V2
 * @version 0.1
 * @date 2021-10-19 (updated 2023-04-02 (by he))
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __BSP_Capture_H
#define __BSP_Capture_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

void BSP_Capture_Init(void);
float BSP_CaptureGetFrequency(uint8_t Channel);
float BSP_CaptureGetDutyRatio(uint8_t Channel);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __BSP_Capture_H */

/*********************************END OF FILE*********************************/
