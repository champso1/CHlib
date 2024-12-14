#include "CHlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const size_t win_w = 640;
const size_t win_h = 480;

int main() {

	InitCHlib(640, 480, "Example 1");

	char input_buf[64];
		
	while (!WindowShouldClose()) {
		// close the window when Q is pressed
		if (IsKeyDown(CL_KEY_Q)) {
			SetWindowShouldClose();
		}
	    
		DrawRectangle(100, 100, 100, 100, CL_COLOR_BLUE);
		DrawRectangle(300, 300, 100, 100, CL_COLOR_RED);
		fprintf(stderr, "\nNumber of shapes: %u\n", RENDER_BATCH.num_shapes);
		RenderAll();

		// advance!
		fgets(input_buf, 64, stdin);
		if (!strncmp(input_buf, "quit", 4)) SetWindowShouldClose();
	}
    
    DeinitCHlib();
	return 0;
}
