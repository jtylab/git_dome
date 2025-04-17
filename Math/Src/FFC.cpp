/**
 * @file FFC.cpp
 * @author jiang
 * @brief 前馈控制
 * @version 0.1
 * @date 2025-04-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "FFC.h"


/**
 * @brief 设置Kf
 * 
 * @param KF 
 */
void FFC_t::Set_Kf(float KF){
    Kf = KF;
}


/**
 * @brief 计算目标值的变化值Target_ROC
 * 
 * @return float 
 */
void FFC_t::Generate(float Target){
    Target_t1 = Target;
    Target_ROC = Target_t0 - Target_t1;
    Target_t0 = Target_t1;
}

/**
 * @brief GetTarget_ROC
 * 
 * @return float 
 */
float FFC_t::GetTatget_ROC(void){
    return Target_ROC;
}

/**
 * @brief GetKf
 * 
 * @return float 
 */
float FFC_t::GetKf(void){
    return Kf;
}