/**
 * @file BSP_KEY.h
 * @brief 板级 KEY 支持包
 * @attention
 * @hardware 
 * @version 0.1
 * @date 2021-07-09
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "main.h"

#ifndef __cplusplus
#include "stdbool.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

bool BSP_KEY_Get(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __BSP_KEY_H */

/*********************************END OF FILE*********************************/
