/**
 * @file BSP_CAN.cpp
 * @brief 板级CAN支持包
 * @attention
 * @hardware 
 * @version 0.2
 * @date 2024-03-24
 * @author tianxu, he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "BSP_CAN.h"

#include "can.h"

#include "string.h"  // memcpy

#define CAN_NUM 2

#define CB_FUNC_NUM     5
#define CB_FUNC_RAW_NUM 2


static BSP_CAN_AllCallbackFunc_t CBF_CAN_ALL[CB_FUNC_NUM] = {0};
static uint8_t CBF_RegNum_CAN_ALL = 0;

// static BSP_CAN_AllCallbackFuncRaw_t CBF_CAN_ALL_Raw[CB_FUNC_RAW_NUM] = {0};
// static uint8_t CBF_RegNum_CAN_ALL_Raw = 0;

static BSP_CAN_CallbackFunc_t CBF_CAN1[CB_FUNC_NUM] = {0};
static BSP_CAN_CallbackFunc_t CBF_CAN2[CB_FUNC_NUM] = {0};
static BSP_CAN_CallbackFuncRaw_t CBF_CAN1_Raw[CB_FUNC_RAW_NUM] = {0};
static BSP_CAN_CallbackFuncRaw_t CBF_CAN2_Raw[CB_FUNC_RAW_NUM] = {0};

static BSP_CAN_Obj_t CAN_Obj[CAN_NUM] = {{&hcan1, CBF_CAN1, CBF_CAN1_Raw, 0}, {&hcan2, CBF_CAN2, CBF_CAN2_Raw, 0}};

__attribute__((aligned(4))) static uint8_t RxData[8];

static CAN_TxHeaderTypeDef TxHeader;
static CAN_RxHeaderTypeDef RxHeader;

/**
 * @brief 初始化CAN总线
 * @note 需要在 main 中调用
 */
void BSP_CAN_Init(void) {
	CAN_FilterTypeDef sFilterConfig;

	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;

	if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_CAN_Start(&hcan1) != HAL_OK) {  // 启动 CAN
		Error_Handler();                   // fatal error
	}
	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
		Error_Handler();
	}
	/*
	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
		Error_Handler();
	}
	*/

	sFilterConfig.FilterBank = 14;
	if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_CAN_Start(&hcan2) != HAL_OK) {  // 启动 CAN
		Error_Handler();                   // fatal error
	}
	if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
		Error_Handler();
	}
	/*
	if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
		Error_Handler();
	}
	*/
}

/**
 * @brief CAN 发送数据
 * @param Port 端口号
 * @param isExt 是否为扩展帧
 * @param isRemote 是否为远程帧
 * @param ID 标准帧报文标识符
 * @param Ext_ID 扩展帧报文标识符
 * @param Data 待发送的数据
 * @param Len 待发送数据的长度，不大于 8
 * @return TxMailbox 使用的发送邮箱编号
 */
void BSP_CAN_SendData(uint8_t Port, bool isExt, bool isRemote, uint32_t ID, uint32_t Ext_ID, uint8_t *Data, uint32_t Len) {
	uint32_t TxMailbox = 0;
	uint8_t idx = Port - 1;

	if (idx > CAN_NUM) return;
	if (Data == NULL) return;
	if (Len > 8) return;

	CAN_HandleTypeDef *hcan = CAN_Obj[idx].hcan;
	
	TxHeader.StdId = ID;
	TxHeader.ExtId = Ext_ID;
	TxHeader.IDE = isExt ? CAN_ID_EXT : CAN_ID_STD;
	TxHeader.RTR = isRemote ? CAN_RTR_REMOTE : CAN_RTR_DATA;
	TxHeader.DLC = Len;
	TxHeader.TransmitGlobalTime = DISABLE;

	if (HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0) {
		HAL_CAN_AbortTxRequest(hcan, 0);
		HAL_CAN_AbortTxRequest(hcan, 1);
		HAL_CAN_AbortTxRequest(hcan, 2);
	}

	if (HAL_CAN_AddTxMessage(hcan, &TxHeader, Data, &TxMailbox)!= HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief CAN 发送标准帧
 * @param Port 端口号
 * @param ID 报文标识符
 * @param Data 待发送的数据
 * @param Len 数据长度
 */
void BSP_CAN_SendStandardData(uint8_t Port, uint32_t ID, uint8_t *Data, uint8_t Len) {
	BSP_CAN_SendData(Port, 0, 0, ID, 0, Data, Len);
}

/**
 * @brief CAN 发送标准帧
 * @param Port 端口号
 * @param ID 报文标识符
 * @param Data 待发送的8字节数据
 */
void BSP_CAN_SendStandard8Data(uint8_t Port, uint32_t ID, uint8_t *Data) {
	BSP_CAN_SendData(Port, 0, 0, ID, 0, Data, 8);
}

/**
 * @brief CAN 发送标准帧
 * @param Port 端口号
 * @param ID 报文标识符
 * @param Data 数据指针
 * @param Len 数据长度
 */
void BSP_CAN_SendRemoteData(uint8_t Port, uint32_t ID, uint8_t *Data, uint8_t Len) {
	BSP_CAN_SendData(Port, 0, 1, ID, 0, Data, Len);
}

/**
 * @brief CAN 发送标准帧
 * @param Port 端口号
 * @param ID 报文标识符
 * @param Data 待发送的8字节数据
 */
void BSP_CAN_SendRemote8Data(uint8_t Port, uint32_t ID, uint8_t *Data) {
	BSP_CAN_SendData(Port, 0, 1, ID, 0, Data, 8);
}

/**
 * @brief 设置 CAN 接收回调函数
 * @param Port 端口号
 * @param Function 函数指针，结构为 void CAN_Callback(uint32_t ID, uint8_t* Data)
 * @return 0 成功
 * @return 1 失败
 */
uint8_t BSP_CAN_SetRxCallbackFunc(uint8_t Port, BSP_CAN_CallbackFunc_t Function) {
	uint8_t idx = Port - 1;
	BSP_CAN_Obj_t *obj = &CAN_Obj[idx];

	if (obj->CBF_RegNum < CB_FUNC_NUM) {
		obj->CBF[obj->CBF_RegNum] = Function;
		obj->CBF_RegNum++;
		return 0;
	} else return 1;
}

/**
 * @brief 设置 CAN 接收回调函数
 * @param Port 端口号
 * @param Function 函数指针，结构为 void CAN_Callback(CAN_RxHeaderTypeDef *RxHeader, uint8_t* Data)
 * @return 0 成功
 * @return 1 失败
 */
uint8_t BSP_CAN_SetRxCallbackFuncRaw(uint8_t Port, BSP_CAN_CallbackFuncRaw_t Function) {
	uint8_t idx = Port - 1;
	BSP_CAN_Obj_t *obj = &CAN_Obj[idx];

	if (obj->CBF_Raw_RegNum < CB_FUNC_RAW_NUM) {
		obj->CBF_Raw[obj->CBF_Raw_RegNum] = Function;
		obj->CBF_Raw_RegNum++;
		return 0;
	} else return 1;
}

/**
 * @brief 设置 CAN 接收回调函数
 * @param Function 函数指针，结构为 void CAN_Callback(uint32_t ID, uint8_t* Data)
 * @return 0 成功
 * @return 1 失败
 */
uint8_t BSP_CAN_SetRxAllCallbackFunc(BSP_CAN_AllCallbackFunc_t Function) {
	if (CBF_RegNum_CAN_ALL < CB_FUNC_NUM) {
		CBF_CAN_ALL[CBF_RegNum_CAN_ALL] = Function;
		CBF_RegNum_CAN_ALL++;
		return 0;
	} else return 1;
}

/**
 * @brief CAN FIFO0 接收回调函数
 * @note 由 HAL 调用，上层无需关心
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
		Error_Handler();
	}
	for (int i = 0; i < CAN_NUM; i++) {
		if (hcan == CAN_Obj[i].hcan && RxHeader.RTR == CAN_RTR_DATA) {
			BSP_CAN_Obj_t *obj = &CAN_Obj[i];
			BSP_CAN_CallbackFunc_t *CBF = obj->CBF;
			BSP_CAN_CallbackFuncRaw_t *CBF_Raw = obj->CBF_Raw;

			for (int i = 0; i < CBF_RegNum_CAN_ALL; i++) {
				if (CBF_CAN_ALL[i] != NULL) CBF_CAN_ALL[i](1, RxHeader.StdId, RxData);
			}
			for (int i = 0; i < obj->CBF_Raw_RegNum; i++) {
				if (CBF_Raw[i] != NULL) CBF_Raw[i](&RxHeader, RxData);
			}
			for (int i = 0; i < obj->CBF_RegNum; i++) {
				if (CBF[i]!= NULL) CBF[i](RxHeader.StdId, RxData);
			}
			break;
		}
	}
}

/**
  * @brief CAN 错误回调函数
  * @note 由 HAL 调用，上层无需关心
  */
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
	// TODO: 将来的错误日志
}

/*********************************END OF FILE*********************************/
