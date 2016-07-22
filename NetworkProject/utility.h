//
// CNH 2016.07.21
// utility functions
//

#pragma once

inline float clamp(float x, float m, float M) {
	return x > M ? M : (x < m ? m : x);
}

inline int clamp(int x, int m, int M) {
	return x > M ? M : (x < m ? m : x);
}

inline float lerp(float a, float b, float x) {
	return (b * x) + (a * (1.0 - x));
}