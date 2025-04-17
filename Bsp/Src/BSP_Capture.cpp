/**
 * @file BSP_Capture.cpp
 * @brief 板级定时器输入捕获支持包
 * @attention 需要在main函数中进行初始化
 * @hardware H-V2
 * @version 0.1
 * @date 2021-10-19 (updated 2023-04-02 (by he))
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "BSP_Capture.h"

#include "tim.h"

#define HTIM htim3

static float Frequency[4] = {0};
static float Duty[4] = {0};

/**
 * @brief 定时器输入捕获初始化
 */
void BSP_Capture_Init(void) {
	HAL_TIM_IC_Start_IT(&HTIM, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&HTIM, TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&HTIM, TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&HTIM, TIM_CHANNEL_4);
}

/**
 * @brief 取通道
 * @param htim 定时器指针
 * @return uint8_t Channel
 * @retval 0 Channel_1
 * @retval 1 Channel_2
 * @retval 2 Channel_3
 * @retval 3 Channel_4
 */
static uint8_t Channel(TIM_HandleTypeDef *htim) {
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
		return 0;
	}
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
		return 1;
	}
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3) {
		return 2;
	}
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) {
		return 3;
	}
	return 255;  // uint8_max
}

/**
 * @brief 取通道2
 * @param CH 通道
 * @return uint32_t TIM_CHANNEL_x
 */
static uint32_t Channel2(uint8_t CH) {
	if (CH == 0) {
		return TIM_CHANNEL_1;
	}
	if (CH == 1) {
		return TIM_CHANNEL_2;
	}
	if (CH == 2) {
		return TIM_CHANNEL_3;
	}
	if (CH == 3) {
		return TIM_CHANNEL_4;
	}
	return 0xFFFFFFFF;  // uint32_max
}

/**
 * @brief 定时器输入捕获中断回调函数
 * @param htim 定时器指针
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	static uint8_t CaptureSta[4] = {0};
	static float CaptureRisingLast[4] = {0};
	static uint32_t CaptureRising[4] = {0};
	static uint32_t CaptureFalling[4] = {0};
	static float Cycle[4] = {0};
	if (htim == &HTIM) {
		uint8_t CH = Channel(htim);
		if (CaptureSta[CH] == 0) {
			if (CaptureRising[CH] != 0) {
				CaptureRisingLast[CH] = CaptureRising[CH];
			}
			CaptureRising[CH] = HAL_TIM_ReadCapturedValue(&HTIM, Channel2(CH));
			if (CaptureRising[CH] < CaptureRisingLast[CH]) {
				Cycle[CH] = 65535 + CaptureRising[CH] - CaptureRisingLast[CH];
			} else {
				Cycle[CH] = CaptureRising[CH] - CaptureRisingLast[CH];
			}
			__HAL_TIM_SET_CAPTUREPOLARITY(&HTIM, Channel2(CH), TIM_ICPOLARITY_FALLING);
			CaptureSta[CH] = 1;
		} else {
			CaptureFalling[CH] = HAL_TIM_ReadCapturedValue(&HTIM, Channel2(CH));
			__HAL_TIM_SET_CAPTUREPOLARITY(&HTIM, Channel2(CH), TIM_ICPOLARITY_RISING);
			CaptureSta[CH] = 0;
			if (CaptureFalling[CH] < CaptureRising[CH]) {
				Duty[CH] = (65535 + CaptureFalling[CH] - CaptureRising[CH]) / Cycle[CH];
			} else {
				Duty[CH] = (CaptureFalling[CH] - CaptureRising[CH]) / Cycle[CH];
			}
			Frequency[CH] = 240000000 / 240 / 2 / Cycle[CH];
		}
	}
}

/**
 * @brief 取输入信号频率
 * @param Channel 通道号(0~3)
 * @return float 频率(Hz)
 */
float BSP_CaptureGetFrequency(uint8_t Channel) {
	return Frequency[Channel];
}

/**
 * @brief 取输入信号占空比
 * @param Channel 通道号(0~3)
 * @return float 占空比(%)
 */
float BSP_CaptureGetDutyRatio(uint8_t Channel) {
	return Duty[Channel];
}

/*********************************END OF FILE*********************************/
