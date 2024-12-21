#include "CHlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const size_t win_w = 640;
const size_t win_h = 480;

int main() {

	InitCHlib(640, 480, "Example 1");

	DrawRectangle(100, 100, 300, 300, CL_COLOR_RED);
		
	while (!WindowShouldClose()) {
		RenderAll();
	}
    
    DeinitCHlib();
	return 0;
}
