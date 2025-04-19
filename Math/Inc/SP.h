/**
 * @file SP.h
 * @author jiang
 * @brief 斜坡规划器
 * @version 0.1
 * @date 2025-04-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #ifndef SP_H
 #define SP_H

#include "main.h"
#include "math.h"

enum Enum_Slope_First{
    Slope_First_REAL = 0,      //真实值优先
    Slope_First_TARGET,        //目标值优先
};


class SP_t{
    private:
        float Increase_Value;  //绝对增量值（加速度）
        float Decrease_Value;  //绝对减量值（减速度）

        float Now_Planning;    // 当前规划值       
        float Output;

    public:
        void Init(float Increase_Value, float Decrease_Value);
        void Slope_Generate(float Now_Real,float Target,Enum_Slope_First Slope_First = Slope_First_REAL);
        float GetOutput(void);

 };


 #ifdef __cplusplus
 extern "C" {
 #endif

 #ifdef __cplusplus






 #endif

#ifdef __cplusplus
} /* extern "C" */
#endif
 

 #ifdef __cplusplus

#endif






















 
 #endif // !SP_H