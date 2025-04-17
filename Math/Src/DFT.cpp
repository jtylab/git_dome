/**
 * @file DFT.cpp
 * @brief 离散傅里叶变换
 * @note 建议转用 CMSIS-DSP 库，提升速度
 * @version 0.1
 * @date 2021-03-04 (updated 2023-04-02 (by he))
 * @author tianxu, he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "DFT.h"

#include "math.h"

#ifndef pi
#define pi 3.1415926535897932384626f
#endif

/**
 * @brief 离散傅里叶变换
 * @param Input 输入数据
 * @param Output 输出数据
 * @param N 点数
 * @param isIDFT 选择是否为逆变换
 */
void DFT(Complex_t *Input, Complex_t *Output, uint32_t N, bool isIDFT) {
	Complex_t temp;
	float s = isIDFT ? -1.0f : 1.0f;
	int k;
	for (k = 0; k < N; k++) {
		Output[k].re = 0;
		Output[k].im = 0;
		for (int n = 0; n < N; n++) {                  // clang-format off
			temp.re =     cos(2.0f * pi * k * n / N);  // clang-format on
			temp.im = s * sin(2.0f * pi * k * n / N);
			Output[k] = Output[k] + (Input[n] * temp);
		}
	}
	if (isIDFT)
		for (k = 0; k < N; k++) {
			Output[k].re /= N;
			Output[k].im /= N;
		}
}

/*********************************END OF FILE*********************************/
