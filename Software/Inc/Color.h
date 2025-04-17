/**
 * @file Color.h
 * @brief 颜色处理
 * @attention
 * @hardware
 * @version 0.1
 * @date 2023-09-27
 *
 */

#ifndef __Color_H
#define __Color_H

#include "main.h"

#ifndef __cplusplus
#include "stdbool.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RGB_u {
    uint16_t R;
    uint16_t G;
    uint16_t B;
} RGB_u;

typedef struct RGB_f {
    float R;
    float G;
    float B;
} RGB_f;

typedef struct HSV_f {
    float H;
    float S;
    float V;
} HSV_f;

void HSV2RGB(HSV_f *HSV, RGB_f *RGB);
void RGB2HSV(RGB_f *RGB, HSV_f *HSV);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __Color_H */

/**********************************END OF FILE*********************************/
