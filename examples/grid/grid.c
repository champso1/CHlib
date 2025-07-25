#include "CHlib/CHlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const size_t win_w = 640;
const size_t win_h = 480;

// #define ALL_BLACK

CLColorRGBAu8 makeColor(u8 x) {
#ifndef ALL_BLACK
	return (CLColorRGBAu8){
		.r = x * 5,
		.g = x * 5,
		.b = x * 5,
		.a = 255,
	};
#else
	return (CLColorRGBAu8){
		.r = 255,
		.g = 0,
		.b = 0,
		.a = 255,
	};
#endif
}

#define GRID_SIZE 4

int main() {

	// make our array of colors
	CLColorRGBAu8 colors[GRID_SIZE * GRID_SIZE] = { 0 };
	for (u8 i=0; i<(GRID_SIZE * GRID_SIZE); i++) {
		colors[i] = makeColor(i);
	}

	InitCHlib(640, 480, "Example Grid");
	MakeGrid(GRID_SIZE, GRID_SIZE, colors);
		
	while (!WindowShouldClose()) {
		RenderAll();
	}
    
    DeinitCHlib();
	return 0;
}
