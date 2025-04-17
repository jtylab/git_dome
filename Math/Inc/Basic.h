/**
 * @file Basic.h
 * @brief 基础数学计算
 * @attention
 * @version 0.1
 * @date 2021-03-04 (updated 2023-04-02 (by he))
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __Basic_H
#define __Basic_H

#include "main.h"

#ifndef pi
#define pi 3.1415926535897932384626f
#endif

#ifdef __cplusplus
extern "C" {
#endif

float LimitMax(float Input, float Max);
float LimitMin(float Input, float Min);
float LimitBoth(float Input, float Max, float Min);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __Basic_H */

/*********************************END OF FILE*********************************/
