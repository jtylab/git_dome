/**
 * @file BSP_CAN.h
 * @brief 板级 CAN 支持包
 * @attention
 * @hardware 
 * @version 0.2
 * @date 2024-03-22
 * @author tianxu, he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __BSP_CAN_H
#define __BSP_CAN_H

#include "main.h"

#ifndef __cplusplus
#include "stdbool.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*BSP_CAN_CallbackFunc_t)(uint32_t ID, uint8_t* Data);
typedef void (*BSP_CAN_CallbackFuncRaw_t)(CAN_RxHeaderTypeDef *RxHeader, uint8_t* Data);
typedef void (*BSP_CAN_AllCallbackFunc_t)(uint8_t Port, uint32_t ID, uint8_t* Data);

typedef struct BSP_CAN_Obj_t {
    CAN_HandleTypeDef *hcan;
    BSP_CAN_CallbackFunc_t *CBF;
    BSP_CAN_CallbackFuncRaw_t *CBF_Raw;
    uint8_t CBF_RegNum;
    uint8_t CBF_Raw_RegNum;
} BSP_CAN_Obj_t;

void BSP_CAN_Init(void);
void BSP_CAN_SendData(uint8_t Port, bool isExt, bool isRemote, uint32_t ID, uint32_t Ext_ID, uint8_t *Data, uint32_t Len);
void BSP_CAN_SendStandardData(uint8_t Port, uint32_t ID, uint8_t *Data, uint8_t Len);
void BSP_CAN_SendStandard8Data(uint8_t Port, uint32_t ID, uint8_t *Data);
void BSP_CAN_SendRemote8Data(uint8_t Port, uint32_t ID, uint8_t *Data);
void BSP_CAN_SendRemoteData(uint8_t Port, uint32_t ID, uint8_t *Data, uint8_t Len);
uint8_t BSP_CAN_SetRxCallbackFunc(uint8_t Port, BSP_CAN_CallbackFunc_t Function);
uint8_t BSP_CAN_SetRxCallbackFuncRaw(uint8_t Port, BSP_CAN_CallbackFuncRaw_t Function);
uint8_t BSP_CAN_SetRxAllCallbackFunc(BSP_CAN_AllCallbackFunc_t Function);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __BSP_CAN_H */

/*********************************END OF FILE*********************************/
