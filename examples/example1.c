#include "CHlib.h"

#include <stdio.h>
#include <stdlib.h>


const size_t win_w = 640;
const size_t win_h = 480;

int main() {

	InitCHlib(640, 480, "Example 1");
	
	while (!WindowShouldClose()) {
		// close the window when Q is pressed
		if (IsKeyDown(CL_KEY_Q)) {
			SetWindowShouldClose();
		}
	    
		DrawRectangle(100, 100, 200, 200, CL_COLOR_BLUE);
		RenderAll();
	}
    
    DeinitCHlib();
	return 0;
}
