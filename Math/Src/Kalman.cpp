/**
 * @file Kalman.cpp
 * @brief 一阶卡尔曼滤波器
 * @attention
 * @version 0.1
 * @date 2021-03-04 (updated 2023-04-02 (by he))
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 */
#include "Kalman.h"

/**
 * @brief 初始化一阶卡尔曼滤波器
 * @param T_Q  过程噪声的协方差
 * @param T_R  测量噪声的协方差
 * @note R固定，Q越大，代表越信任侧量值，Q无穷代表只用测量值
 *       反之，Q越小代表越信任模型预测值，Q为零则是只用模型预测
 */
void KalmanFilter_t::Init(float T_Q, float T_R) {
	Q = T_Q;  // 过程噪声的协方差
	R = T_R;  // 测量噪声的协方差
}

/**
 * @brief 初始化一阶卡尔曼滤波器
 * @param T_Q  过程噪声的协方差
 * @param T_R  测量噪声的协方差
 * @param x_0  初始值
 * @note R固定，Q越大，代表越信任侧量值，Q无穷代表只用测量值
 *       反之，Q越小代表越信任模型预测值，Q为零则是只用模型预测
 */
void KalmanFilter_t::Init(float T_Q, float T_R, float x_0) {
	Q = T_Q;  // 过程噪声的协方差
	R = T_R;  // 测量噪声的协方差
	X_last = x_0;
	X_now = x_0;
}

/**
 * @brief 一阶卡尔曼滤波器计算
 * @return 计算结果
 */
float KalmanFilter_t::Generate(float data) {
	X_mid = A * X_last;
	P_mid = A * P_last + Q;
	Kg = P_mid / (P_mid + R);
	X_now = X_mid + Kg * (data - X_mid);
	P_now = (1 - Kg) * P_mid;

	X_last = X_now;
	P_last = P_now;
	return X_now;
}

/**
 * @brief 一阶卡尔曼滤波器取计算结果
 * @return 计算结果
 */
float KalmanFilter_t::GetOutput(void) {
	return X_now;
}

/*********************************END OF FILE*********************************/
