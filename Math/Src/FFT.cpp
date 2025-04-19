/**
 * @file FFT.cpp
 * @brief 快速傅里叶变换
 * @note 建议转用 CMSIS-DSP 库，提升速度
 * @version 0.1
 * @date 2023-01-08
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */

/**
 *  https://b23.tv/BV1Y7411W73U
 */

#include "FFT.h"

#include "math.h"
#include "stdlib.h"
#include "string.h"

#ifndef __PI
#define __PI 3.1415926535897932384626f
#endif

static void swap(Complex_t *a, Complex_t *b) {
	Complex_t t = *a;
	*a = *b;
	*b = t;
}

// static uint32_t rev[4096] = {0};

/**
 * @brief 快速傅里叶变换
 * @param x 数据
 * @param N 点数 一般为2的幂次
 * @param isIFFT 选择是否为逆变换
 */
void FFT(Complex_t *x, uint32_t N, bool isIFFT) {
	uint32_t i, size = 1, pow = 0, *rev;
	float s = isIFFT ? -1.0f : 1.0f;
	while (size < N) size <<= 1, pow++;                 // FFT 需要 点数size 为 2 的幂次
	rev = (uint32_t *)malloc(sizeof(uint32_t) * size);  // 分配内存空间， rev[] 用于产生 位逆序置换(BitReverse) 所需序列
	if (rev == NULL) {
		Error_Handler();
	}
	memset(rev, 0, sizeof(uint32_t) * size);
	for (i = 0; i < size; ++i) rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (pow - 1));  // 生成序列
	for (i = 0; i < size; ++i)
		if (i < rev[i]) swap(&x[i], &x[rev[i]]);  // 进行 位逆序置换(BitReverse)
	free(rev);
	for (i = 1; i <= pow; ++i) {
		uint32_t m1 = 1 << i;
		Complex_t wn = Complex_t(cos(2.0f * __PI / m1), s * sin(2.0f * __PI / m1));  // 单位根 ω_n
		for (uint32_t k = 0; k < size; k += m1) {
			Complex_t w = Complex_t(1.0f, 0.0f);
			for (uint32_t j = 0; j < m1 / 2; ++j) {
				Complex_t t = w * x[k + j + m1 / 2];  // clang-format off
				Complex_t u =     x[k + j];
				x[k + j]         = u + t;  // clang-format on
				x[k + j + m1 / 2] = u - t;
				w = w * wn;
			}
		}
	}
	if (isIFFT)
		for (i = 0; i < size; ++i) x[i].re /= size;
}

/*********************************END OF FILE*********************************/
