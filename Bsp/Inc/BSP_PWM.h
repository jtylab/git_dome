/**
 * @file BSP_PWM.h
 * @brief 板级 PWM 支持包
 * @attention
 * @hardware 
 * @version 0.1
 * @date 2021-10-29 (updated 2024-03-22 (by he))
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

void BSP_PWM_Init(void);
void BSP_PWM_Set(uint8_t Code, uint32_t Val);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __BSP_PWM_H */

/*********************************END OF FILE*********************************/
