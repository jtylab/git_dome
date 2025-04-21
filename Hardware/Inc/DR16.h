/**
 * @file DR16.h
 * @brief DR16 接收机
 * @attention 依赖 BSP_UART
 * @hardware 
 * @version 0.1
 * @date 2022-03-13 (updated 2024-03-22 (by he))
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __DR16_H
#define __DR16_H

#include "main.h"
#include "stdbool.h"

#ifndef __cplusplus
#endif

#define REMOTE_SW_UP   1
#define REMOTE_SW_MID  3
#define REMOTE_SW_DOWN 2

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
	/* rocker channel information */
	int16_t CH1;
	int16_t CH2;
	int16_t CH3;
	int16_t CH4;
	/* left and right lever information */
	uint8_t SW1;
	uint8_t SW2;
	/* mouse movement and button information */
	struct {
		int16_t X;
		int16_t Y;
		int16_t Z;
		uint8_t L;
		uint8_t R;
	} Mouse;
	/* keyboard key information */
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
	int16_t Wheel;
	uint8_t Online;
} DR16_t;

extern bool DR16_OnlineFlag;

void DR16_Init(void);
DR16_t *DR16_Point(void);
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* __DR16_H */

/*********************************END OF FILE*********************************/
