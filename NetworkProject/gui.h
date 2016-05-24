//
// CNH 2016.05.22
// GUI definition
//

#pragma once

#include "protocol.h"

#define GUI_CELL_WIDTH 48.0
#define GUI_CELL_HEIGHT 48.0
#define GUI_MAP_X (WINDOW_WIDTH / 2.0)
#define GUI_MAP_Y (WINDOW_HEIGHT / 2.0)

class Gui {
private:
	static float animationPhase, animationFullPhase, animationIndpPhase;
	static int imin(int a, int b) { return a > b ? b : a; }
public:
	static float unitX(float x) { return GUI_MAP_X + (x - (MAP_WIDTH - 1) / 2.0) * GUI_CELL_WIDTH; }
	static float unitY(float y) { return GUI_MAP_Y + (y - (MAP_HEIGHT - 1) / 2.0) * GUI_CELL_HEIGHT; }
	static void turn();
	static void update();
	static float aniPhase() { return animationPhase; }
	static float aniFullPhase() { return animationFullPhase; }
	static float aniIndpPhase() { return animationIndpPhase; }
	static int aniPhaseCombinate(int num) { return imin((int)(animationPhase * num), num - 1); }
	static int aniFullPhaseCombinate(int num) { return imin((int)(animationFullPhase * num), num - 1); }
	static int aniIndpPhaseCombinate(int num, float rate) { return (int)(animationIndpPhase * rate) % num; }
};