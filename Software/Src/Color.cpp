/**
 * @file Color.cpp
 * @brief 颜色处理
 * @attention
 * @hardware
 * @version 0.1
 * @date 2023-09-27
 *
 */

#include "Color.h"

#include "math.h"

float max3(float a, float b, float c) {
    if (a > b) {
        if (a > c) return a;
        else return c;
    } else {
        if (b > c) return b;
        else return c;
    }
}

float min3(float a, float b, float c) {
    if (a < b) {
        if (a < c) return a;
        else return c;
    } else {
        if (b < c) return b;
        else return c;
    }
}

void HSV2RGB(HSV_f *HSV, RGB_f *RGB) {
    float R,G,B;
    float C = HSV->V * HSV->S;
    float H_prime = HSV->H * 360.0f / 60.0f;
    float X = C * (1 - abs(fmod(H_prime, 2.0f) - 1));
    if (0 <= H_prime && H_prime < 1) {
        R = C;
        G = X;
        B = 0;
    } else if (1 <= H_prime && H_prime < 2) {
        R = X;
        G = C;
        B = 0;
    } else if (2 <= H_prime && H_prime < 3) {
        R = 0;
        G = C;
        B = X;
    } else if (3 <= H_prime && H_prime < 4) {
        R = 0;
        G = X;
        B = C;
    } else if (4 <= H_prime && H_prime < 5) {
        R = X;
        G = 0;
        B = C;
    } else if (5 <= H_prime && H_prime < 6) {
        R = C;
        G = 0;
        B = X;
    } else return;
    float m = HSV->V - C;
    RGB->R = R + m;
    RGB->G = G + m;
    RGB->B = B + m;
}

void RGB2HSV(RGB_f *RGB, HSV_f *HSV) {
    float H_prime;
    HSV->V = max3(RGB->R, RGB->G, RGB->B);  // X_max
    float X_min = min3(RGB->R, RGB->G, RGB->B);  // V - C
    float C = HSV->V - X_min;
    if (C == 0) H_prime = 0;
    else if (HSV->V == RGB->R) {
        H_prime = fmod((RGB->G - RGB->B) / C, 6.0f);
    } else if (HSV->V == RGB->R) {
        H_prime = (RGB->B - RGB->R) / C + 2.0f;
    } else if (HSV->V == RGB->R) {
        H_prime = (RGB->R - RGB->G) / C + 4.0f;
    } else return;
    HSV->H = 60.0f * H_prime / 360.0f;
    if (HSV->V == 0) HSV->S = 0;
    else HSV->S = C / HSV->V;
}
