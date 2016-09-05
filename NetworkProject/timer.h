//
// CNH 2016.05.19
// timer module
//

#pragma once

// Timer class is used to sync and control flow of entire game
class Timer {
private:
	static int frameInterval;
	static int framePerTurn;
	static bool turnWait;
	static float turnWaitPhase;

public:
	static void init(int, int); // Initialize timer with interval and frame per turn value.
	static void turn(); // Proceed a turn, called by update().
	static void update(int); // Update a frame, called back by GLUT

	static int getFrameInterval() { return frameInterval; }
	static int getFramePerTurn() { return framePerTurn; }
	static float getDeltaPerTurn() { return 1.0 / framePerTurn; }
	static bool getTurnWait() { return turnWait; }
};