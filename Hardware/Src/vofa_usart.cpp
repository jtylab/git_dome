/**
 * @file vofa_usart.cpp
 * @author jiang
 * @brief vofa实时调参
 * @version 0.1
 * @date 2025-08-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "vofa_usart.h"

struct PID_Data_t PID_Data;

void vofa_usart_RxCallBack(uint8_t *Data, uint32_t Len){
   
    if(Data[0] == One_flag && Data[1] == Two_flag){
        switch(Data[2]){
            case kp_flag:
                memcpy(&PID_Data.kp,&Data[3],4);
                PID_Data.kp = (int)(PID_Data.kp * 100) / 100.0f;
                break;

            case ki_flag:
                memcpy(&PID_Data.ki,&Data[3],4);
                PID_Data.ki = (int)(PID_Data.ki * 100) / 100.0f;
                break;

            case kd_flag:
                memcpy(&PID_Data.kd,&Data[3],4);
                PID_Data.kd = (int)(PID_Data.kd * 100) / 100.0f;
                break;
        }
    }
}


void vofa_usart_Init(void){
    BSP_UART_SetRxCallbackFunc(uart_Point, vofa_usart_RxCallBack);
}
 


PID_Data_t* GetPID_DataPoint(void){
    return &PID_Data;
}