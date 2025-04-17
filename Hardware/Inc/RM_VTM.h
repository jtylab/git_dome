/**
 * @file RM_VTM.h
 * @brief 图传遥控控制链路通信
 * @attention
 * @hardware H-V1 H-V2
 * @version 0.1
 * @date 2023-04-19
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __VTM_H
#define __VTM_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
#include "stdbool.h"
#endif

#pragma pack(1)
typedef struct {
	struct {
		int16_t X;
		int16_t Y;
		int16_t Z;

		uint8_t L;
		uint8_t R;
	} Mouse;
	union {
		uint16_t KeyCode;
		struct {
			uint16_t W : 1;
			uint16_t S : 1;
			uint16_t A : 1;
			uint16_t D : 1;
			uint16_t SHIFT : 1;
			uint16_t CTRL : 1;
			uint16_t Q : 1;
			uint16_t E : 1;
			uint16_t R : 1;
			uint16_t F : 1;
			uint16_t G : 1;
			uint16_t Z : 1;
			uint16_t X : 1;
			uint16_t C : 1;
			uint16_t V : 1;
			uint16_t B : 1;
		} Key;
	} KeyBoard;
	uint16_t Reserved;
} VTM_Raw_t;

typedef struct {
	VTM_Raw_t Raw;
	float MoveX;
	float MoveY;
	float Yaw;
	float Pitch;
	bool isOnline;
} VTM_t;
#pragma pack()

void VTM_Init(void);
VTM_t *VTM_Point(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __VTM_H */

/*********************************END OF FILE*********************************/
