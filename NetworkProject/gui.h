//
// CNH 2016.05.22
// GUI definition
//

#pragma once

#include "protocol.h"

#define ACTUAL_WINDOW_WIDTH 1280
#define ACTUAL_WINDOW_HEIGHT 960
#define INFO_WINDOW_WIDTH 360
#define INFO_WINDOW_HEIGHT 240

#define GUI_CELL_WIDTH 48.0
#define GUI_CELL_HEIGHT 48.0
#define GUI_MAP_X (WINDOW_WIDTH / 2.0)
#define GUI_MAP_Y (WINDOW_HEIGHT / 2.0 - 48.0)

// Gui module defines various graphical values
// Such as animation phase, map size, and so on.
class Gui {
private:
	static float animationPhase; // phase from the begin to the end of the movement
	static float animationPostPhase; // phase from the end of the movement to the end of the turn
	static float animationFullPhase; // phase from the begin to the end of the turn
	static float animationIndpPhase; // phase independent from turn flow
	static int imin(int a, int b) { return a > b ? b : a; }
public:
	static float unitX(float x) { return GUI_MAP_X + (x - (MAP_WIDTH - 1) / 2.0) * GUI_CELL_WIDTH; }
	static float unitY(float y) { return GUI_MAP_Y + (y - (MAP_HEIGHT - 1) / 2.0) * GUI_CELL_HEIGHT; }
	static void turn();
	static void update();
	static float aniPhase() { return animationPhase; }
	static float aniPostPhase() { return animationPostPhase; }
	static float aniFullPhase() { return animationFullPhase; }
	static float aniIndpPhase() { return animationIndpPhase; }
	static int aniPhaseCombinate(int num) { return imin((int)(animationPhase * num), num - 1); }
	static int aniPostPhaseCombinate(int num) { return imin((int)(animationPostPhase * num), num - 1); }
	static int aniFullPhaseCombinate(int num) { return imin((int)(animationFullPhase * num), num - 1); }
	static int aniIndpPhaseCombinate(int num, float rate) { return (int)(animationIndpPhase * rate) % num; }
	static bool isPost() { return animationPhase >= 1.0; }
};