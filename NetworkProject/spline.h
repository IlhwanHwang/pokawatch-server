//
// CNH 2016.05.22
// various splines
//

#pragma once

#include <cmath>

#define M_PI 3.1415926
#define HAISTNESS 0.5

// Splines are used for making smooth animations
class Spline {
public:
	static float sigmoid(float x) { return 0.5 - 0.5 * cosf(x * M_PI); } // smooth enter and exit
	static float accandfric(float x) { return sigmoid(powf(x, HAISTNESS)); } // faster enter and slower exit
	static float jump(float x) { return sinf(x * M_PI); } // jump
	static float accjump(float x) { return jump(powf(x, HAISTNESS)); } // faster jump and slower land
	static float sqr(float x) { return 1.0 - (1.0 - x) * (1.0 - x);  }
};
