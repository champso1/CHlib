#include "CHlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const size_t win_w = 640;
const size_t win_h = 480;

int main() {

	InitCHlib(640, 480, "Example 1");
		
	while (!WindowShouldClose()) {
		// close the window when Q is pressed
		if (IsKeyDown(CL_KEY_Q)) {
			SetWindowShouldClose();
		}
	    
		DrawRectangle(100, 100, 100, 100, CL_COLOR_BLUE);
		DrawRectangle(300, 300, 100, 100, CL_COLOR_RED);
		RenderAll();
	}
    
    DeinitCHlib();
	return 0;
}
