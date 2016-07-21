//
// CNH 2016.07.21
// utility functions
//

#pragma once

float clamp(float x, float m, float M) {
	return x > M ? M : (x < m ? m : x);
}

int clamp(int x, int m, int M) {
	return x > M ? M : (x < m ? m : x);
}