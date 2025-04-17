/**
 * @file CRC.cpp
 * @brief CRC 校验
 * @attention
 * @hardware H-V1 H-V2
 * @version 0.1
 * @date 2023-04-13
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __CRC_H
#define __CRC_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t Get_CRC8(uint8_t *Data, uint32_t Length);
uint16_t Get_CRC16(uint8_t *Data, uint32_t Length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CRC_H */

/*********************************END OF FILE*********************************/
