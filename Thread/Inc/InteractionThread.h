/**
 * @file InteractionThread.h
 * @brief 交互线程
 * @attention
 * @hardware H-V1 H-V2
 * @version 0.1
 * @date 2021-07-26
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __InteractionThread_H
#define __InteractionThread_H

#include "main.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"


// #include "BSP_KEY.h"
#include "BSP_Buzzer.h"
#include "BSP_LED_PWM.h"

#include "DR16.h"

#include "Color.h"

#include "DebuggerThread.h"
#include "Robodefine.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
class Interaction_t {
	private:
		uint32_t Private_SystemState;

	public:
		Interaction_t() {
			Private_SystemState = 0;
		}
		void SetSystemState(uint32_t SystemState);
		friend uint32_t InteractionGetSystemState(void);
};
#endif

void InteractionThread_Init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __InteractionThread_H */

/*********************************END OF FILE*********************************/
