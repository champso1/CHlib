#include "CHlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const size_t win_w = 640;
const size_t win_h = 480;


clColorRGBAu8 makeColor(u8 x) {
	return (clColorRGBAu8){
		.r = x * 5,
		.g = x * 5,
		.b = x * 5,
		.a = 255,
	};
}

#define GRID_SIZE 5

int main() {

	// make our array of colors
	clColorRGBAu8 colors[GRID_SIZE * GRID_SIZE] = { 0 };
	for (u8 i=0; i<(GRID_SIZE * GRID_SIZE); i++) {
		colors[i] = makeColor(i);
	}

	InitCHlib(640, 480, "Example 1");
	MakeGrid(GRID_SIZE, GRID_SIZE, colors);
		
	while (!WindowShouldClose()) {
		RenderAll();
	}
    
    DeinitCHlib();
	return 0;
}
