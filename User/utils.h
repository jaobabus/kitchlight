#pragma once
#ifndef USER_UTILS_H_
#define USER_UTILS_H_



inline int32_t sin16(int16_t x) {
    // x-x^3/6+x^5/150
    // x=[-180;180]
    // sin16(x)=[-32768;32768]
    int32_t x2 = x * x;
    int32_t xk = x * 32768;
    int32_t x4 = x2 * x2;
    int32_t r = 57;
    int32_t r2 = r * r;
    volatile int32_t res = xk / 57 - xk / r * (x2 / 6 - x4 / 150 / r2) / r2;
    return res;
}


#endif /* USER_UTILS_H_ */
