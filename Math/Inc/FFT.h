/**
 * @file FFT.h
 * @brief 快速傅里叶变换
 * @note 建议转用 CMSIS-DSP 库，提升速度
 * @version 0.1
 * @date 2023-01-08
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __FFT_H
#define __FFT_H

#include "main.h"

#include "Complex.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
void FFT(Complex_t *x, uint32_t N, bool isIFFT);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __FFT_H */

/*********************************END OF FILE*********************************/
