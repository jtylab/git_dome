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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * @brief 设置Kf
 * 
 * @param KF 
 */
void FFC_Target_t::Set_Kf(float KF){
    Kf = KF;
}

/**
 * @brief 计算目标值的变化值Target_ROC
 * 
 * @return float 
 */
void FFC_Target_t::Generate(float Target){
    Target_t1 = Target;
    Target_ROC = Target_t0 - Target_t1;
    Target_t0 = Target_t1;
}

/**
 * @brief GetTarget_ROC
 * 
 * @return float 
 */
float FFC_Target_t::GetTatget_ROC(void){
    return Target_ROC;
}

/**
 * @brief GetKf
 * 
 * @return float 
 */
float FFC_Target_t::GetKf(void){
    return Kf;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * @brief 初始化阻力前馈系数
 * 
 * @param a0 
 * @param a1 
 * @param a2 
 * @param a3 
 * @param Firstordercontinuous_limiting 
 * @param Feedforward_limiting 
 */
void FFC_Resistance_t::Init(float a0, float a1, float a2, float a3, float Firstordercontinuous_limiting, float Feedforward_limiting){
    FFC_Resistance_t::a0 = a0;
    FFC_Resistance_t::a1 = a1;
    FFC_Resistance_t::a2 = a2;
    FFC_Resistance_t::a3 = a3;
    FFC_Resistance_t::Firstordercontinuous_limiting = Firstordercontinuous_limiting;
    FFC_Resistance_t::Feedforward_limiting = Feedforward_limiting;
}

/**
 * @brief 计算阻力补偿值
 * 
 * @param Z 
 */
void FFC_Resistance_t::Generate(float Z){
    if(abs(Z) < Firstordercontinuous_limiting){
        Resistance_Value = a0 * Z * (Z < 0 ?-1: Z > 0 ?1: 0);                                        //???????????????????????????????????????????一阶连续函数
    }
    else{
        Resistance_Value = LimitMax((a0 + a1 * abs(Z) + a2 * Z * Z),Feedforward_limiting) * (Z < 0 ?-1: Z > 0 ?1: 0);
    }
}

/**
 * @brief Get阻力补偿值
 * 
 * @return float 
 */
float FFC_Resistance_t::GetResistance(void){
    return Resistance_Value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * @brief 初始化
 * 
 * @param LU_L0         //LU轮子的力距
 * @param RU_L1         //RU轮子的力距
 * @param LD_L2         //LD轮子的力距
 * @param RD_L3         //RD轮子的力距
 * @param Slope_Pitch   //斜坡的Pitch倾角{-pi,pi}
 */
void FFC_Gravity_t::Init(float LU_L0, float RU_L1, float LD_L2, float RD_L3, float Slope_Pitch){
    FFC_Gravity_t::ForceDistance[LU] = LU_L0;
    FFC_Gravity_t::ForceDistance[RU] = RU_L1;
    FFC_Gravity_t::ForceDistance[LD] = LD_L2;
    FFC_Gravity_t::ForceDistance[RD] = RD_L3;

    FFC_Gravity_t::Slope_Pitch = Slope_Pitch;

    FFC_Gravity_t::Generate();
}

/**
 * @brief 计算每个电机的重力补偿
 * 
 */
void FFC_Gravity_t::Generate(void){

    //依据公式:
    //         N0*L0 = N1*L1 = N2*L2 = N3*L3
    //         N0 + N1 + N2 + N3 = mgcos(Slope_pitch)
    float Sum_inverse_L = 1.0/ForceDistance[LU] + 1.0/ForceDistance[RU] + 1.0/ForceDistance[LD] + 1.0/ForceDistance[RD];
    float K = m*g * cos(Slope_Pitch) / Sum_inverse_L;

    float Supportiveness[4];           //每个电机的支持力N
    Supportiveness[LU] = K / ForceDistance[LU];
    Supportiveness[RU] = K / ForceDistance[RU];
    Supportiveness[LD] = K / ForceDistance[LD];
    Supportiveness[RD] = K / ForceDistance[RD];

    float WeightFactor[4];             //支持力权重系数n
    float Sum_resultant_F = Supportiveness[LU] + Supportiveness[RU] + Supportiveness[LD] + Supportiveness[RD];
    WeightFactor[LU] = Supportiveness[LU] / Sum_resultant_F;
    WeightFactor[RU] = Supportiveness[RU] / Sum_resultant_F;
    WeightFactor[LD] = Supportiveness[LD] / Sum_resultant_F;
    WeightFactor[RD] = Supportiveness[RD] / Sum_resultant_F;

    GravityCompensation[LU] = m*g * cos(Slope_Pitch) * WeightFactor[LU];
    GravityCompensation[RU] = m*g * cos(Slope_Pitch) * WeightFactor[RU];
    GravityCompensation[LD] = m*g * cos(Slope_Pitch) * WeightFactor[LD];
    GravityCompensation[RD] = m*g * cos(Slope_Pitch) * WeightFactor[RD];
}

/**
 * @brief Get每个电机的重力补偿
 * 
 * @return float* 
 */
float* FFC_Gravity_t::GetCompensation(void){
    return GravityCompensation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////