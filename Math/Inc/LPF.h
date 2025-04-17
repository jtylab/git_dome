/**
 * @file LPF.h
 * @brief 一阶低通滤波器
 * @attention
 * @version 0.1
 * @date 2021-09-25 (updated 2023-04-02 (by he))
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __LPF_H
#define __LPF_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
class LPF_t {
	private:
		float Private_T;
		float Private_Frequency;
		float Private_Output;

	public:
		LPF_t();
		LPF_t(float T, float Frequency);
		void Init(float T, float Frequency);
		float Generate(float Input);
		float GetOutput(void);
};
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __LPF_H */

/*********************************END OF FILE*********************************/
