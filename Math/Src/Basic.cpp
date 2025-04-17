/**
 * @file Basic.cpp
 * @brief 基础数学计算
 * @attention
 * @version 0.1
 * @date 2021-06-02 (updated 2023-04-02 (by he))
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "Basic.h"

/**
 * @brief 最大限幅
 * @param Input 输入
 * @param Max 最大限幅值
 * @return 限幅结果
 */
float LimitMax(float Input, float Max) {
	return ((Input > Max) ? (Max) : (Input));
}

/**
 * @brief 最小限幅
 * @param Input 输入
 * @param Min 最小限幅值
 * @return 限幅结果
 */
float LimitMin(float Input, float Min) {
	return ((Input < Min) ? (Min) : (Input));
}

/**
 * @brief 最大最小限幅
 * @param Input 输入
 * @param Max 最大限幅值
 * @param Min 最小限幅值
 * @return 限幅结果
 */
float LimitBoth(float Input, float Max, float Min) {
	return LimitMax(LimitMin(Input, Min), Max);
}

/*********************************END OF FILE*********************************/
