/**
 * @file PID.h
 * @brief 位置式 PID 控制器，微分项带一阶低通滤波
 * @attention
 * @version 0.2
 * @date 2024-03-05
 * @author tianxu, he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __PID_H
#define __PID_H

#include "main.h"
#include "math.h"
#include "Basic.h"

#include "FFC.h"
#include "BSP_UART.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
class PID_t {
	private:
		float Private_P;               // PID系数P
		float Private_I;               // PID系数I
		float Private_D;               // PID系数D
		float Private_T;               // 计算周期(单位ms)

		float Private_Limit_Integral;  // 积分限幅
		float Private_Limit_Output;    // 输出限幅

		float Private_Deadzone;    // 死区值


		float Private_Integral;
		float Private_Last_Error;
		float Private_Last_Differ;


		float Private_LPF_q;

		float Private_Output;  // 输出

		FFC_Target_t TargetFeedforward;

	public:
		PID_t() {
			Private_P = 0;
			Private_I = 0;
			Private_D = 0;
			Private_T = 1;

			Private_Limit_Integral = -1;
			Private_Limit_Output = -1;

			Private_Deadzone = 0;

			Private_Integral = 0;
			Private_Last_Error = 0;
			Private_Last_Differ = 0;

			Private_LPF_q = 2.0f * pi * (Private_T / 1000.0f) * 30.0f;  // 截止频率 30 Hz

			Private_Output = 0;
		}
		void Init(float Kp, float Ki, float Kd, float Kf,float T, float Limit_Integral, float Limit_Output, float Deadzone = 0);
		float GenerateRing(float Input, float Target, float Perimeter);
		float Generate(float Input, float Target);
		void SetPID(float P, float I, float D);
		void SetLimit(float Limit_Integral, float Limit_Output);
		void SetDeadzone(float Deadzone);
		void ClearIntegral(void);
		float GetIntegral(void);
		float GetOutput(void);
		void Vofa_Set_kpid(float kp , float ki, float kd);
};
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __PID_H */

/*********************************END OF FILE*********************************/
