/**
 * @file BSP_LED_PWM.h
 * @brief 板级 LED 支持包
 * @attention
 * @hardware
 * @version 0.1
 * @date 2023-07-27
 * @author tianxu, he
 * @copyright Copyright (c) LNPU TCN
 *
 */

#ifndef __BSP_LED_PWM_H
#define __BSP_LED_PWM_H

#include "main.h"

#ifndef __cplusplus
#include "stdbool.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void BSP_LEDPWM_Init(void);
void BSP_LEDPWM_CR_Val(uint32_t val);
void BSP_LEDPWM_CG_Val(uint32_t val);
void BSP_LEDPWM_CB_Val(uint32_t val);
void BSP_LEDPWM_RGB_Val(uint32_t R, uint32_t G, uint32_t B);
void BSP_LEDPWM_RGB_Valf(float R, float G, float B);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __BSP_LED_PWM_H */

/**********************************END OF FILE*********************************/
