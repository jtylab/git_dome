/**
 * @file Complex.h
 * @brief 复数
 * @attention
 * @version 0.1
 * @date 2021-03-04 (updated 2023-04-02 (by he))
 * @author tianxu
 * @copyright Copyright (c) LNPU TCN
 *
 */

#ifndef __Complex_H
#define __Complex_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
class Complex_t {
	public:
		float re;
		float im;
		Complex_t() {
			re = 0.0f;
			im = 0.0f;
		}
		Complex_t(float r, float i) {
			re = r;
			im = i;
		}
		Complex_t operator+(const Complex_t& b) {
			Complex_t c;
			c.re = this->re + b.re;
			c.im = this->im + b.im;
			return c;
		}
		Complex_t operator-(const Complex_t& b) {
			Complex_t c;
			c.re = this->re - b.re;
			c.im = this->im - b.im;
			return c;
		}
		Complex_t operator*(const Complex_t& b) {
			Complex_t c;
			c.re = this->re * b.re - this->im * b.im;
			c.im = this->im * b.re + this->re * b.im;
			return c;
		}
		Complex_t operator/(const Complex_t& b) {
			Complex_t up;
			float down;
			up.re = b.re;
			up.im = -b.im;
			up = (*this) * (up);
			down = b.re * b.re + b.im * b.im;
			up.re /= down;
			up.im /= down;
			return up;
		}
};
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __Complex_H */

/*********************************END OF FILE*********************************/
