//
// CNH 2016.05.19
// timer module
//

#pragma once

// timer module working in static context
class Timer {
private:
	static int frameInterval;
	static int framePerTurn;

public:
	static void init(int, int);
	static void turn();
	static void update(int);

	static int getFrameInterval() { return frameInterval; }
	static int getFramePerTurn() { return framePerTurn; }
};
