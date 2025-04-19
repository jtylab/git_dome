/**
 * @file SP.cpp
 * @author jiang
 * @brief 斜坡规划器
 * @version 0.1
 * @date 2025-04-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */


 #include "SP.h"



/**
 * @brief 初始化
 * 
 * @param Increase_Value        //绝对增量值（加速度）
 * @param Decrease_Value        //绝对减量值（减速度）
 */
void SP_t::Init(float Increase_Value, float Decrease_Value){
    SP_t::Increase_Value = Increase_Value;
    SP_t::Decrease_Value = Decrease_Value;
}

 /**
  * @brief 斜坡规划计算函数
  * 
  * @param Now_Real       //当前真实值
  * @param Target         //目标值
  * @param Slope_First    //规划类型
  */
 void SP_t::Slope_Generate(float Now_Real,float Target,Enum_Slope_First Slope_First){
    switch (Slope_First){
        case Slope_First_REAL:
            if((Target >= Now_Real && Now_Real >= Now_Planning) || (Target <= Now_Real && Now_Real <= Now_Planning)){
                Output = Now_Real;
            }
        
            break;

        case Slope_First_TARGET:

            break;

        default:
            break;
    }

    if(Now_Planning > 0.0f){
            if(Target > Now_Planning){
                if(abs(Now_Planning - Target) > Increase_Value){
                    Output += Increase_Value;
                }
                else{
                    Output = Target;
                }
            }
            else if(Target < Now_Planning){
                if(abs(Now_Planning - Target) > Decrease_Value){
                    Output -= Decrease_Value;
                }
                else{
                    Output = Target;
                }
            }
        }
        else if(Now_Planning < 0.0f){
            if(Target < Now_Planning){
                if(abs(Now_Planning -Target) > Increase_Value){
                    Output -= Increase_Value;
                }
                else{
                    Output = Target;
                }
            }
            else if(Target > Now_Planning){
                if(abs(Now_Planning - Target) > Decrease_Value){
                    Output += Decrease_Value;
                }
                else{
                    Output = Target;
                }
            }
        }
        else{
            if(Target > Now_Planning){
                if(abs(Now_Planning - Target) > Increase_Value){
                    Output += Increase_Value;
                }
                else{
                    Output = Target;
                }
            }
            else if(Target < Now_Planning){
                if(abs(Now_Planning -Target) > Increase_Value){
                    Output -= Increase_Value;
                }
                else{
                    Output = Target;
                }
            }
        }

    Now_Planning = Output;
    
 }

/**
 * @brief Get斜坡规划输出值
 * 
 * @return float 
 */
 float SP_t::GetOutput(void){
    return Output;
 }
