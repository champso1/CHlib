#ifndef _CHLIB_H
#define _CHLIB_H


#include "glad/glad.h"
#include "GLFW/glfw3.h"



// --------------------
// BASIC DEFINES / GENERIC UTILS
// --------------------

typedef unsigned short chglError;
typedef unsigned short GLFWError;
typedef unsigned char u8;

#define GLFW_OKAY                      0 // All is well!
#define GLFW_ERROR                     1 // Generic GLFW Error

#define CHGL_OKAY                      0 // All is well!
#define CHGL_ERROR                     1 // Generic CHGL error


typedef enum _chglBool {false = 0, true = 1} chglBool;




// --------------------
// Math-related Stuff
// --------------------

struct _chglVec2f { // Basic vector type
	float x, y;
};
typedef struct _chglVec2f chglVec2f;

// converts from (0,win_[w,h]) to (-1,1)
// assumes window coordinate's origin is at the top left
extern chglVec2f coordsWindowToGLFW(chglVec2f in, int win_w, int win_h);


struct _chglVec4u8 { // 4-bytes
	u8 r, g, b, a;
};
typedef struct _chglVec4u8 chglVec4u8;
typedef struct _chglVec4u8 chglColor;
typedef struct _chglVec4u8 chglColoru8;


struct _chglVec4f {
	float r, g, b, a;
};
typedef struct _chglVec4f chglVec4f;
typedef struct _chglVec4f chglColorf;


// converts color from 0-255 to 0.0-1.0
extern chglColorf colorNormalize(chglColoru8 color);



// Some default colors:
#define CHGL_COLOR_BLACK     (chglColoru8){0, 0, 0, 0}
#define CHGL_COLOR_WHITE     (chglColoru8){0xFF, 0xFF, 0xFF, 0xFF}
#define CHGL_COLOR_LIGHTGRAY (chglColoru8){0x18, 0x18, 0x18, 0x18}

#define CHGL_DEFAULT_BG_COLOR CHGL_COLOR_LIGHTGRAY



// --------------------
// GLFW Default Callbacks
// --------------------

extern void defaultFramebufferSizeCallback(GLFWwindow* window, int width, int height);
extern void defaultErrorCallback(int error, const char* description);
extern void defaultKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);



// --------------------
// GLFW Initialization / CHlib internal funcs
//
// NOTE: `chgl` prefixed funcs don't interact with the global state.
//       They are meant to interact ONLY with GLFW/GL
// --------------------

// Initializes glfw in the normal way (with defaults) up until window creation
// and returns the created window.
extern GLFWwindow* chglInitGLFW(int width, int height, const char* title);
// Deinitializes glfw in the normal way and sets global states to defaults.
extern void chglDeinitGLFW(GLFWwindow* window);

// Internal function to create default shader program.
extern GLuint chglCreateDefaultShaderProgram();
// Internal function to delete default shader program.
extern void chglDeleteDefaultShaderProgram(GLuint shader_prog);




// --------------------
// Global State Definitions
// --------------------

typedef struct _chglState {
	GLuint default_shader_prog;

	chglBool is_initialized;
	GLFWwindow* window_handle;

	int win_w;
	int win_h;

	chglColorf current_bg_color;
} chglState;

extern chglState GLOBAL_STATE;




// --------------------
// Final API
//
// NOTE: These are the only functions that are not camelCase
//       They also _do_ (expectedly) interact with the global state.
//       This is done via passing global state pointers to `chgl`
//         prefixed funcs.
//       They are still allowed to interact with GLFW/GL
// --------------------

// initializes GLFW and some global state variables
// a valid rendering context/GL function pointers are set here
// essentially everything that is needed to do a draw call
extern chglError InitCHlib(int win_w, int win_h, const char* title);
// renders all RenderObjects within the current RenderBatch
// as well as any other glfw loop-by-loop basis
extern chglError RenderAll();
// frees all memory and GLFW-related stuff
extern chglError DeinitCHlib();

// Returns false (meaning the window should stay open)
// so long as nothing has set it to so so
extern chglBool WindowShouldClose();
// closes window, basically
extern void     SetWindowShouldClose();

// Clears the background with the given color
extern chglError SetBackgroundColor(chglColor color);




struct _chglCallbacks {
	void (*framebuffer_size_callback)(GLFWwindow*, int, int);
	void (*error_callback)(int, const char*);
	void (*key_callback)(GLFWwindow*, int, int, int, int);
};
typedef struct _chglCallbacks chglCallbacks;



extern void SetCallbacks(chglCallbacks callbacks);



#endif // _CHLIB_H
