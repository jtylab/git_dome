/**
 * @file VOFA.cpp
 * @brief VOFA
 * @attention
 * @hardware 
 * @version 0.1
 * @date 2024-03-05
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "VOFA.h"

#include "BSP_UART.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "timers.h"


#define VOFA_UART_Port 4
#define VOFA_UART_Baud 2000000

#define VOFA_MAX_CH 31


static float VOFA_Buff[VOFA_MAX_CH + 1] = {0};
static uint8_t VOFA_UsingCH = 0;

osTimerId_t VOFA_osTimerId;
const static osTimerAttr_t VOFA_osTimerAttr = {"VOFA_osTimer", 0, NULL, 0};
static void VOFA_osTimer_CbHandler(void *argument);

void VOFA_Init(void)
{
    *(uint32_t *)&VOFA_Buff[VOFA_MAX_CH] = 0x7F800000UL;
    BSP_UART_SetBaudrate(VOFA_UART_Port, VOFA_UART_Baud);
    VOFA_osTimerId = osTimerNew(VOFA_osTimer_CbHandler, osTimerPeriodic, NULL, &VOFA_osTimerAttr);
    osTimerStart(VOFA_osTimerId, 1);  // 1ms
}

void VOFA_SetChValue(uint8_t ch, float value)
{
    if (ch == 0 || ch > VOFA_MAX_CH) return;
    if (ch > VOFA_UsingCH) VOFA_UsingCH = ch;
    VOFA_Buff[ch - 1] = value;
}

void VOFA_osTimer_CbHandler(void *argument)
{
    if (VOFA_UsingCH == 0) return;
    *(uint32_t *)&VOFA_Buff[VOFA_UsingCH] = 0x7F800000UL;
    BSP_UART_SendMessage(VOFA_UART_Port, (uint8_t *)VOFA_Buff, (VOFA_UsingCH + 1) * 4);
}

/*********************************END OF FILE*********************************/
