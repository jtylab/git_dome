/**
 * @file TFmini.h
 * @brief TFmini激光测距
 * @attention
 * @hardware H-V1 H-V2
 * @version 0.1
 * @date 2023-04-05
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __TFmini_H
#define __TFmini_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)
typedef struct {
	uint8_t Head[2];  // 0x59, 0x59
	uint16_t Distance;
	uint16_t Strength;
	uint16_t TempRaw;
	uint8_t Sum;
	uint16_t Temp;  // Temp = TempRaw / 8 - 256
} TFmini_t;
#pragma pack()

void TFmini_Init(void);
TFmini_t *TFmini_Point(uint8_t Code);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __TFmini_H */

/*********************************END OF FILE*********************************/
