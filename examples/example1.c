#include "CHlib.h"

#include <stdio.h>
#include <stdlib.h>


const size_t win_w = 640;
const size_t win_h = 480;

int main() {

	InitCHlib(640, 480, "Window Title");

	while (!WindowShouldClose()) {
		RenderAll();
	}
    
    DeinitCHlib();
	return 0;
}
