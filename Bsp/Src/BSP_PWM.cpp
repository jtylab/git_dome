/**
 * @file BSP_PWM.cpp
 * @brief 板级 PWM 支持包
 * @attention 需要在 main 函数中进行初始化
 * @hardware 
 * @version 0.1
 * @date 2021-10-29 (updated 2024-03-22 (by he))
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "BSP_PWM.h"

#include "tim.h"


#define BSP_PWM_MaxVal 9999


/**
 * @brief PWM 输出初始化
 * @note 需要在 main 中调用
 */
void BSP_PWM_Init(void) {
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
	// HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

/**
 * @brief 设置 PWM 占空比
 * @param Code 接口编号 0 ~ 7
 * @param Val 定时器溢出值，决定占空比 范围为 0 ~ BSP_PWM_MaxVal = 9999
 * @note Code = 7 为激光接口
 */
void BSP_PWM_Set(uint8_t Code, uint32_t Val) {
	if (Val > BSP_PWM_MaxVal) {
		Val = BSP_PWM_MaxVal;
	}
	switch (Code) {
		case 0:
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, Val);
			break;
		case 1:
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, Val);
			break;
		case 2:
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, Val);
			break;
		case 3:
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, Val);
			break;
		case 4:
			__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, Val);
			break;
		case 5:
			__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, Val);
			break;
		case 6:
			__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, Val);
			break;
		case 7:
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, Val);
			break;
	}
}

/*********************************END OF FILE*********************************/
