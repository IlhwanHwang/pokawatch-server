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
public:
	static float unitX(float x) { return GUI_MAP_X + (x - (MAP_WIDTH - 1) / 2.0) * GUI_CELL_WIDTH; }
	static float unitY(float y) { return GUI_MAP_Y + (y - (MAP_HEIGHT - 1) / 2.0) * GUI_CELL_HEIGHT; }
};