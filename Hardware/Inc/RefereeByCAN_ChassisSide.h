/**
 * @file RefereeByCAN_ChassisSide.h
 * @brief RoboMaster 裁判系统通信
 * @attention
 * @hardware
 * @version 
 * @date 2024-03-24
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __RefereeByCAN_ChassisSide_H
#define __RefereeByCAN_ChassisSide_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

void RefByCAN_RobotStatus_TxCbHandler(void);
void RefByCAN_ChassisPower_TxCbHandler(void);
void RefByCAN_Heat_TxCbHandler(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __RefereeByCAN_ChassisSide_H */

/*********************************END OF FILE*********************************/
