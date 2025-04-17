/**
 * @file BSP_KEY.cpp
 * @brief 板级 KEY 支持包
 * @attention
 * @hardware 
 * @version 0.1
 * @date 2021-07-09
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "BSP_KEY.h"

/**
 * @brief 读取 KEY 按键状态
 * @return 1按下 0松开
 */
bool BSP_KEY_Get(void) {
	return (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET);
}

/*********************************END OF FILE*********************************/
