//
// CNH 2016.05.22
// various splines
//

#pragma once

#include <cmath>

#define M_PI 3.1415926
#define HAISTNESS 0.5

class Spline {
public:
	static float sigmoid(float x) { return 0.5 - 0.5 * cosf(x * M_PI); }
	static float accandfric(float x) { return sigmoid(powf(x, HAISTNESS)); }
	static float jump(float x) { return sinf(x * M_PI); }
	static float accjump(float x) { return jump(powf(x, HAISTNESS)); }
};
