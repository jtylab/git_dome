/**
 * @file BSP_ADC.cpp
 * @brief 板级ADC支持包
 * @attention 需要在 main 函数中进行初始化
 * @hardware 
 * @version 0.1
 * @date 2023-07-09
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "BSP_ADC.h"

#include "adc.h"

#include "BSP_UART.h"

#define ADC_CH_NUM 1

const float ADC_Vref = 3.3f;  // Vref (Volt)
const float ADC_VrefH = ADC_Vref * 0.5f;  // half Vref (Volt)
const float ADC_LSB = ADC_Vref / 4095.0f;  // 1LSB (Volt)

static uint16_t ADC_Val[ADC_CH_NUM] = {0};
static float ADC_Voltage[ADC_CH_NUM] = {0};

/**
 * @brief 初始化 BSP_ADC
 */
void BSP_ADC_Init(void) {
	HAL_Delay(5);  //等待校准电压稳定
	// HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc3, (uint32_t*)ADC_Val, ADC_CH_NUM);
}

/**
 * @brief 取 ADC 原始值
 * @param 通道号（0~ADC_CH_NUM）
 * @return ADC原始值
 */
uint16_t BSP_ADC_GetVal(uint8_t CH) {
	return ADC_Val[CH];
}

/**
 * @brief 取 ADC 电压值
 * @param 通道号（0~ADC_CH_NUM）
 * @return 电压(单位V)
 */
float BSP_ADC_GetVoltage(uint8_t CH) {
	return ADC_Voltage[CH];
}

/**
 * @brief 取 ADC 电压数组指针
 * @return 电压数组指针(值的单位为V)
 * @retval float *
 */
float *BSP_ADC_GetVoltageP(void) {
	return ADC_Voltage;
}

/**
 * @brief ADC转换完成中断
 * @note 由 HAL 调用，上层无需关心
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc == &hadc3) {
		for (int i = 0; i < ADC_CH_NUM; i++) {
			ADC_Voltage[i] = (float)(ADC_Val[i])*ADC_LSB;
		}
	}
}

/*********************************END OF FILE*********************************/
