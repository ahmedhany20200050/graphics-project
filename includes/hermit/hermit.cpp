//
// Created by metr on 5/15/2023.
//
#include <windows.h>
#include "hermit.h"
#include <cmath>
int myHermitRound(double num) {
    return (int) round(num);
}
void performHermiteCurveAlgorithm(HDC hdc, double x1, double y1, double u1, double v1, double x2, double y2, double u2, double v2, COLORREF c){

    double a3 = 2*x1+ u1 -2* x2 + u2;
    double a2 = -3 * x1 -2 * u1 + 3* x2 - u2;
    double a1 = u1;
    double a0 = x1;

    double b3 =  2*y1+ v1 -2 * y2 + v2;
    double b2 = -3 * y1 -2 * v1 + 3* y2 - v2;
    double b1 = v1;
    double b0 = y1;

    for (double t =0.0; t <= 1.0 ;){

        double x = (a3 * t *t *t) + (a2 * t *t) + (a1 *t) + a0;
        double y = (b3 * t *t *t) + (b2 *t *t) + (b1 *t) + b0;
        SetPixel(hdc, myHermitRound(x), myHermitRound(y),c);
        t+= 0.001;
    }
}