/**
 * @file BSP_ADC.h
 * @brief 板级ADC支持包
 * @attention 
 * @hardware 
 * @version 0.1
 * @date 2023-07-09
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

void BSP_ADC_Init(void);
uint16_t BSP_ADC_GetVal(uint8_t CH);
float BSP_ADC_GetVoltage(uint8_t CH);
float *BSP_ADC_GetVoltageP(void);
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __BSP_ADC_H */

/*********************************END OF FILE*********************************/
