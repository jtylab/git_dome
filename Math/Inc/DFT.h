/**
 * @file DFT.h
 * @brief 离散傅里叶变换
 * @note 建议转用 CMSIS-DSP 库，提升速度
 * @version 0.1
 * @date 2021-05-28 (updated 2023-04-02 (by he))
 * @author tianxu, he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __DFT_H
#define __DFT_H

#include "main.h"

#include "Complex.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
void DFT(Complex_t *Input, Complex_t *Output, uint32_t N, bool isIDFT);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __DFT_H */

/*********************************END OF FILE*********************************/
