/**
 * @file VOFA.h
 * @brief VOFA
 * @attention
 * @hardware 
 * @version 0.1
 * @date 2024-03-05
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __VOFA_H
#define __VOFA_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

void VOFA_Init(void);
void VOFA_SetChValue(uint8_t ch, float value);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __VOFA_H */

/*********************************END OF FILE*********************************/
