/**
 * @file BSP_LEDPWM.cpp
 * @brief 板级 LED 支持包
 * @attention
 * @hardware
 * @version 0.1
 * @date 2023-07-27
 * @author tianxu, he
 * @copyright Copyright (c) LNPU TCN
 *
 */

#include "BSP_LED_PWM.h"

#include "tim.h"

#define PWM_MaxVal 65535

/**
 * @brief PWM 输出初始化
 * @note 需要在 main 中调用
 */
void BSP_LEDPWM_Init(void) {
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);  // B
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);  // G
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);  // R
}

/**
 * @brief 设置 RGB LED 红色
 * @param val 值 (0~PWM_MaxVal)
 */
void BSP_LEDPWM_CR_Val(uint32_t val) {
	__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, val);
}

/**
 * @brief 设置 RGB LED 绿色
 * @param val 值 (0~PWM_MaxVal)
 */
void BSP_LEDPWM_CG_Val(uint32_t val) {
	__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, val);
}

/**
 * @brief 设置 RGB LED 蓝色
 * @param val 值 (0~PWM_MaxVal)
 */
void BSP_LEDPWM_CB_Val(uint32_t val) {
	__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, val);
}

/**
 * @brief 设置 RGB LED
 * @param R 值 (0~PWM_MaxVal)
 * @param G 值 (0~PWM_MaxVal)
 * @param B 值 (0~PWM_MaxVal)
 */
void BSP_LEDPWM_RGB_Val(uint32_t R, uint32_t G, uint32_t B) {
	BSP_LEDPWM_CR_Val(R);
	BSP_LEDPWM_CG_Val(G);
	BSP_LEDPWM_CB_Val(B);
}

/**
 * @brief 设置 RGB LED
 * @param R 值 (0~1)
 * @param G 值 (0~1)
 * @param B 值 (0~1)
 */
void BSP_LEDPWM_RGB_Valf(float R, float G, float B) {
	BSP_LEDPWM_CR_Val(R * PWM_MaxVal);
	BSP_LEDPWM_CG_Val(G * PWM_MaxVal);
	BSP_LEDPWM_CB_Val(B * PWM_MaxVal);
}

/**********************************END OF FILE*********************************/
