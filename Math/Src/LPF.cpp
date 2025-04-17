/**
 * @file LPF.cpp
 * @brief 一阶低通滤波器
 * @attention 依赖 Basic.h
 * @version 0.1
 * @date 2021-09-25 (updated 2023-04-02 (by he))
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 */

#include "LPF.h"

#include "Basic.h"

/**
 * @brief 一阶低通滤波器构造函数
 */
LPF_t::LPF_t() {
	Private_T = 0.0f;
	Private_Frequency = 0.0f;
	Private_Output = 0.0f;
}

/**
 * @brief 初始化一阶低通滤波器
 * @param T 采样周期 (单位 ms)
 * @param Frequency 截止频率 (单位 Hz)
 */
void LPF_t::Init(float T, float Frequency) {
	Private_T = (T > 0.0f ? T : 0.0f) / 1000.0f;              // 小于等于零代表不滤波
	Private_Frequency = Frequency > 0.0f ? Frequency : 0.0f;  // 小于等于零代表不滤波
}

/**
 * @brief 一阶低通滤波器计算
 * @param Input 输入数据
 */
float LPF_t::Generate(float Input) {
	if (Private_T <= 0.0f || Private_Frequency <= 0.0f) {
		Private_Output = Input;
		return Private_Output;
	}
	float LPF_q = LimitBoth(2.0f * pi * Private_T * Private_Frequency, 1.0f, 0.0f);
	Private_Output = LPF_q * Input + (1.0f - LPF_q) * Private_Output;
	return Private_Output;
}

/**
 * @brief 一阶低通滤波器取输出值
 */
float LPF_t::GetOutput(void) {
	return Private_Output;
}

/*********************************END OF FILE*********************************/
