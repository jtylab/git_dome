/**
 * @file BSP_Buzzer.h
 * @brief 板级无源蜂鸣器支持包
 * @attention
 * @hardware 
 * @version 0.1
 * @date 2023-07-09
 * @author tianxu, he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __BSP_Buzzer_H
#define __BSP_Buzzer_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

void BSP_Buzzer_Init(void);
void BSP_Buzzer_Set(float Val);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __BSP_Buzzer_H */

/*********************************END OF FILE*********************************/
