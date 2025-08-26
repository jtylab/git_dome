/**
 * @file InteractionThread.cpp
 * @brief 交互线程
 * @attention
 * @hardware H-V1 H-V2
 * @version 0.1
 * @date 2021-07-26
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "InteractionThread.h"


Interaction_t Interaction;

static uint32_t LED_tick = 0;
static const uint32_t LED_TickReset = 1000;

static HSV_f HSV;
static RGB_f RGB;

/**
 * @brief 人机交互设置系统状态
 */
void Interaction_t::SetSystemState(uint32_t SystemState) {
	Private_SystemState = SystemState;
}

/**
 * @brief 获取系统状态
 * @attention 友元函数
 */
uint32_t InteractionGetSystemState(void) {
	return Interaction.Private_SystemState;
}

/**
 * @brief LED线程函数
 */
void LED_Task(void *argument) {
	while (1) {
		HSV.H = (float)LED_tick / (float)LED_TickReset;
		HSV.S = 1.0f;
		HSV.V = 1.0f;
		HSV2RGB(&HSV, &RGB);
		BSP_LEDPWM_RGB_Valf(RGB.R, RGB.G, RGB.B);

		LED_tick++;
		if (LED_tick > LED_TickReset) {
			LED_tick = 0;
		}
		osDelay(5);
	}
}

void Buzzer(uint32_t Val, uint32_t Time) {
	BSP_Buzzer_Set(Val);
	osDelay(Time);
	BSP_Buzzer_Set(0);
}

/**
 * @brief 蜂鸣器线程函数
 */
void Buzzer_Task(void *argument) {
	osDelay(1000);

	// Buzzer(400, 300);
	// Buzzer(1000, 300);
	// Buzzer(1600, 300);
	 
	// Chord F
	Buzzer(349.228, 125);   // 523 C5     349.228 F4
	Buzzer(440.000, 125);   // 659 E5     440.000 A4
	Buzzer(523.251, 125);   // 784 G5     523.251 C5

	osThreadExit();

	while (1) {
		osDelay(1000);
	}
}

osThreadId_t LEDThreadHandle;
osThreadId_t BuzzerThreadHandle;
const osThreadAttr_t LEDTask_attributes = {"LEDTask", 0, 0, 0, 0, 256, (osPriority_t)osPriorityLow};
const osThreadAttr_t BuzzerTask_attributes = {"BuzzerTask", 0, 0, 0, 0, 256, (osPriority_t)osPriorityLow};
/**
 * @brief 线程初始化
 */
void InteractionThread_Init(void) {
	LEDThreadHandle = osThreadNew(LED_Task, NULL, &LEDTask_attributes);
	BuzzerThreadHandle = osThreadNew(Buzzer_Task, NULL, &BuzzerTask_attributes);
}

/*********************************END OF FILE*********************************/
