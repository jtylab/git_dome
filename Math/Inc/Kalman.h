/**
 * @file Kalman.h
 * @brief 一阶卡尔曼滤波器
 * @attention
 * @version 0.1
 * @date 2021-05-28 (updated 2023-04-02 (by he))
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 */
#ifndef __KALMAN_H
#define __KALMAN_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
class KalmanFilter_t {
	private:
		float X_last;
		float X_mid;
		float X_now;
		float P_last;
		float P_mid;
		float P_now;
		float Kg;
		float A;
		float B;
		float Q;
		float R;
		float H;

	public:
		KalmanFilter_t() {
			X_last = 0;
			X_mid = 0;
			P_last = 0;
			A = 1;
			B = 0;
			H = 1;
		}
		void Init(float T_Q, float T_R);
		void Init(float T_Q, float T_R, float x0);
		float Generate(float data);
		float GetOutput(void);
};
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __KALMAN_H */

/*********************************END OF FILE*********************************/
