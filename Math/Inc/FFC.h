/**
 * @file FFC.h
 * @author jiang
 * @brief 前馈控制
 * @version 0.1
 * @date 2025-04-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #ifndef FFC_H
 #define FFC_H

 #include "main.h"
 #include "math.h"
 #include "Basic.h"
 
 #ifdef __cplusplus
 extern "C" {
 #endif

 #ifdef __cplusplus

 #define LU 0
 #define RU 1
 #define LD 2
 #define RD 3

#define g   9.834       //重力加速度
#define m   1           //????????????????????????????????????????????????????????机器人重力


 /**
  * @brief 期望前馈补偿
  * 
  */
 class FFC_Target_t{
    private:
        float Kf;
        float Target_t0;
        float Target_t1;
        float Target_ROC;

    public:
    FFC_Target_t(){
            Target_t0 = 0.0f;
            Target_t1 = 0.0f;
            Target_ROC = 0.0f;
            Kf = 0.0f;
        }
        void Set_Kf(float Kf);
        void Generate(float Target);
        float GetTatget_ROC(void);
        float GetKf(void);

};

/**
 * @brief 阻力前馈补偿
 * 
 */
class FFC_Resistance_t{
    private:
        //电机角速度于电机摩擦力一阶连续函数f(x)系数
        float a0; 
        //拟合的电机角速度于电机摩擦力一阶函数g(x)系数        
        float a1;
        float a2;
        float a3;

        float Firstordercontinuous_limiting;    //一阶连续限幅（防止零点震荡）
        float Feedforward_limiting;             //阻力前馈限幅

        float Resistance_Value;

    public:
        FFC_Resistance_t(void){
            a0 = 0;
            a1 = 0;
            a2 = 0;
            a3 = 0;
            Firstordercontinuous_limiting = 0;
            Feedforward_limiting = 0;
            Resistance_Value = 0;
        }

        void Init(float a0, float a1, float a2, float a3, float Firstordercontinuous_limiting, float Feedforward_limiting);
        void Generate(float Z);
        float GetResistance(void);
  
};

/**
 * @brief 重力前馈补偿
 * 
 */
class FFC_Gravity_t{
    private:
        float ForceDistance[4];
        float GravityCompensation[4];    //电机在斜坡上的重力补偿
        float Slope_Pitch;

        float Feedforward_limiting;

    public:
        void Init(float L0, float L1, float L2, float L3, float Slope_Pitch,float Feedforward_limiting);
        void Generate(void);
        float* GetCompensation(void);
};










#endif



#ifdef __cplusplus
} /* extern "C" */
#endif
 
 #endif // !FFC_H