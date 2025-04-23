/**
 * @file PID.cpp
 * @brief 位置式 PID 控制器，微分项带一阶低通滤波
 * @attention 依赖项 Basic
 * @version 0.2
 * @date 2024-03-05
 * @author tianxu, he
 * @copyright Copyright (c) LNPU TCN
 *
 */

#include "PID.h"


/**
 * @brief PID 控制器初始化
 * 
 * @param Kp PID比例系数 Kp
 * @param Ki PID积分系数 Ki
 * @param Kd PID微分系数 Kd
 * @param Kf FFC前馈控制系数 Kf
 * @param T 采样周期 (单位ms)
 * @param Limit_Integral 积分限幅 (不使用置为 -1)
 * @param Limit_Output   输出限幅 (不使用置为 -1)
 * @param Deadzone       双向死区值 (不使用置 0)
 * 
 */
void PID_t::Init(float Kp, float Ki, float Kd, float Kf,float T, float Limit_Integral, float Limit_Output, float Deadzone) {
	Private_P = Kp;
	Private_I = Ki;
	Private_D = Kd;
	TargetFeedforward.Set_Kf(Kf);
	Private_T = T;
	Private_LPF_q = 2.0f * pi * (Private_T / 1000.0f) * 5.0f;  // 一阶低通的截止频率为 30Hz
	Private_Limit_Integral = Limit_Integral;
	Private_Limit_Output = Limit_Output;
	Private_Deadzone = Deadzone;
}

/**
 * @brief PID 控制器计算
 * @param Input 输入值
 * @param Target 目标值
 * @return (float) 输出值
 */
float PID_t::Generate(float Input, float Target) {

	TargetFeedforward.Generate(Target);   //更新Target_ROC
	float Target_ROC = TargetFeedforward.GetTatget_ROC();
	float FFC_Kf = TargetFeedforward.GetKf();
	float Error;   // 误差
	float Differ;  // 微分项
	Error = Target - Input;                                                                                   // 当前误差等于目标值减去当前值


	if ((Private_I != 0) && (Private_T != 0)) {                                                               // 检查积分项
		Private_Integral += Private_I * Private_T * Error;                                                    // 计算积分项
		if (Private_Limit_Integral >= 0) {                                                                    // 限幅小于零表示不限幅
			Private_Integral = LimitBoth(Private_Integral, Private_Limit_Integral, -Private_Limit_Integral);  // 积分项限幅
		}
	}
	if ((Private_D != 0) && (Private_T != 0)) {  // 检查微分项
		if (isnan(Private_Last_Differ)) {        // 变量异常保护
			Differ = 0;
			Private_Last_Differ = 0;
		} else {
			Differ = (Error - Private_Last_Error) / Private_T;  // 计算微分项

			Differ = Private_LPF_q * Differ + (1 - Private_LPF_q) * Private_Last_Differ;  // 对微分项进行一阶低通滤波

			Private_Last_Error = Error;    // 更新误差记录
			Private_Last_Differ = Differ;  // 更新微分项记录
		}
	}
	Private_Output = Private_P * Error + Private_Integral + Private_D * Differ + FFC_Kf * Target_ROC ;
	if (Private_Limit_Output >= 0) {                                                              // 限幅小于零表示不限幅
		Private_Output = LimitBoth(Private_Output, Private_Limit_Output, -Private_Limit_Output);  // 输出值限幅
	}
	return Private_Output;
}

/**
 * @brief PID 控制器圆周计算
 * @param Input 输入值
 * @param Target 目标值
 * @param Perimeter 周长
 * @return (float) 输出值
 * @note 仅在电机于角度模式时使用，且需要符合这两种情况之一：电机装配完成后能够全范围转动或者转动角度小于180度，否则需要另作特殊处理
 */
float PID_t::GenerateRing(float Input, float Target, float Perimeter) {
	float Error;   // 误差
	float Differ;  // 微分项
	Error = Target - Input;       // 当前误差等于目标值减去当前值

    if(abs(Error) <= Private_Deadzone){
		Error = 0;
	}

	if (Error > Perimeter / 2) {  // 处理连续旋转的相对误差
		Error -= Perimeter;
	} else if (Error < (-Perimeter) / 2) {
		Error += Perimeter;
	}
	if ((Private_I != 0) && (Private_T != 0)) {                                                               // 检查积分项
		Private_Integral += Private_I * Private_T * Error;                                                    // 计算积分项
		if (Private_Limit_Integral >= 0) {                                                                    // 限幅小于零表示不限幅
			Private_Integral = LimitBoth(Private_Integral, Private_Limit_Integral, -Private_Limit_Integral);  // 积分项限幅
		}
	}
	if ((Private_D != 0) && (Private_T != 0)) {  // 检查微分项
		if (isnan(Private_Last_Differ)) {        // 变量异常保护
			Differ = 0;
			Private_Last_Differ = 0;
		} else {
			Differ = (Error - Private_Last_Error) / Private_T;  // 计算微分项

			Differ = Private_LPF_q * Differ + (1 - Private_LPF_q) * Private_Last_Differ;  // 对微分项进行一阶低通滤波

			Private_Last_Error = Error;    // 更新误差记录
			Private_Last_Differ = Differ;  // 更新微分项记录
		}
	}
	Private_Output = Private_P * Error + Private_Integral + Private_D * Differ ;
	if (Private_Limit_Output >= 0) {                                                              // 限幅小于零表示不限幅
		Private_Output = LimitBoth(Private_Output, Private_Limit_Output, -Private_Limit_Output);  // 输出值限幅
	}
	return Private_Output;
}

/**
 * @brief PID 控制器设置 PID 系数
 * @param P 比例系数 P
 * @param I 积分系数 I
 * @param D 微分系数 D
 */
void PID_t::SetPID(float P, float I, float D) {
	Private_P = P;
	Private_I = I;
	Private_D = D;
	Private_LPF_q = 2.0f * pi * (Private_T / 1000.0f) * 5.0f;
}

/**
 * @brief PID 控制器设置限制
 * @param Limit_Integral 积分限幅 (不使用置为 -1)
 * @param Limit_Output   输出限幅 (不使用置为 -1)
 */
void PID_t::SetLimit(float Limit_Integral, float Limit_Output) {
	Private_Limit_Integral = Limit_Integral;
	Private_Limit_Output = Limit_Output;
}

/**
 * @brief PID 控制器设置双向死区
 * @param Deadzone 双向死区值 (不使用置 0)
 * 
 */
void PID_t::SetDeadzone(float Deadzone) {
	Private_Deadzone = Deadzone;

}

/**
 * @brief PID 控制器积分值清零
 */
void PID_t::ClearIntegral(void) {
	Private_Integral = 0;
}

/**
 * @brief PID 控制器取积分值
 * @return (float) 积分值
 */
float PID_t::GetIntegral(void) {
	return Private_Integral;
}

/**
 * @brief PID 控制器获取输出值
 * @return (float) 输出值
 */
float PID_t::GetOutput(void) {
	return Private_Output;
}

/*********************************END OF FILE*********************************/
