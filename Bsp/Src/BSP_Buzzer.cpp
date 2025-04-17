/**
 * @file BSP_Buzzer.cpp
 * @brief 板级无源蜂鸣器支持包
 * @attention 需要在 main 函数中进行初始化
 * @hardware 
 * @version 0.1
 * @date 2023-07-09
 * @author tianxu, he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "BSP_Buzzer.h"

#include "tim.h"

#define ARR_Value 110.0
#define BUZZER1_HTIM &htim4
#define BUZZER1_CHANNEL TIM_CHANNEL_3

/**
 * @brief 蜂鸣器初始化
 * @note 需要在 main 中调用
 */
void BSP_Buzzer_Init(void) {
	HAL_TIM_PWM_Start(BUZZER1_HTIM, BUZZER1_CHANNEL);
	__HAL_TIM_SetCompare(BUZZER1_HTIM, BUZZER1_CHANNEL, 0);
}

/**
 * @brief 设置蜂鸣器频率
 * @param Val 频率 (Hz)
 */
void BSP_Buzzer_Set(float Val) {
	if (Val == 0) {
		__HAL_TIM_SetCompare(BUZZER1_HTIM, BUZZER1_CHANNEL, 0);
		return;
	}
	__HAL_TIM_SET_PRESCALER(BUZZER1_HTIM, (int)(HAL_RCC_GetPCLK1Freq()/ARR_Value/Val)-1);  // round
	__HAL_TIM_SET_AUTORELOAD(BUZZER1_HTIM, (int)ARR_Value-1);
	__HAL_TIM_SetCompare(BUZZER1_HTIM, BUZZER1_CHANNEL, (int)(ARR_Value/2)-1);
}

/*********************************END OF FILE*********************************/
