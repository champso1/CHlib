#ifndef _CHLIB_H
#define _CHLIB_H


#include "glad/glad.h"
#include "GLFW/glfw3.h"





/*************************************************************/
//-----------------------------------------------------------//
// All defines, enums, structs, and such.
//-----------------------------------------------------------//
/*************************************************************/




// --------------------
//       TYPES
// --------------------

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

typedef char i8;
typedef short i16;
typedef int i32;
typedef long i64;

typedef float f32;
typedef double f64;

enum _clError {CL_OKAY = 0, CL_ERROR = 1};
typedef enum _clError clError;

enum _clBool  {false = 0, true = 1};
typedef enum _clBool clBool;




// -------------------------
//   Vectors/Colors/Math
// ------------------------

struct _clVec2f { // Basic vector type
	union {
		struct { f32 x, y; };
		struct { f32 w, h; };
	};
};
typedef struct _clVec2f clVec2f;
typedef struct _clVec2f clPoint2f;


struct _clVec2u32 {
	union {
		struct { u32 x, y; };
		struct { u32 w, h; };
	};
};
typedef struct _clVec2u32 clVec2u32;
typedef struct _clVec2u32 clPoint2u32;


struct _clVec3f {
	union {
		struct { float x, y, z; };
		struct { float r, g, b; };
	};
};
typedef struct _clVec3f clVec3f;
typedef struct _clVec3f clPoint3f;
typedef struct _clVec3f clColorRGBf;


// not really feasible to define this as a point,
// since it only goes up to 255
struct _clVec3u8 {
	u8 r, g, b;
};
typedef struct _clVec3u8 clVec3u8;
typedef struct _clVec3u8 clColorRGBu8;


struct _clVec3u32 {
	u32 x, y, z;
};
typedef struct _clVec3u32 clVec3u32;
typedef struct _clVec3u32 clPoint3u32;


struct _clVec4u8 {
	u8 r, g, b, a;
};
typedef struct _clVec4u8 clVec4u8;
typedef struct _clVec4u8 clColorRGBAu8;


struct _clVec4f {
	float r, g, b, a;
};
typedef struct _clVec4f clVec4f;
typedef struct _clVec4f clColorRGBAf;



// Some default colors:
#define CL_COLOR_BLACK       (clColorRGBAu8){0, 0, 0, 0}
#define CL_COLOR_WHITE       (clColorRGBAu8){0xFF, 0xFF, 0xFF, 0xFF}
#define CL_COLOR_BLUE        (clColorRGBAu8){0x00, 0x00, 0xFF, 0xFF}
#define CL_COLOR_GREEN       (clColorRGBAu8){0x00, 0xFF, 0x00, 0xFF}
#define CL_COLOR_RED         (clColorRGBAu8){0xFF, 0x00, 0x00, 0xFF}
#define CL_COLOR_DARKGRAY    (clColorRGBAu8){0x18, 0x18, 0x18, 0x18}

#define CL_DEFAULT_BG_COLOR  CL_COLOR_DARKGRAY


// used when comparing floats
#define CL_MIN_FLOAT_THRESHOLD 0.00001f



// --------------------
//       INPUT
// --------------------

// shamelessly stolen from GLFW and Raylib
#define CL_KEY_NONE            0   // no key pressed
// "Printable" keys
#define CL_KEY_SPACE           32
#define CL_KEY_APOSTROPHE      39
#define CL_KEY_COMMA           44
#define CL_KEY_MINUS           45
#define CL_KEY_PERIOD          46
#define CL_KEY_SLASH           47
#define CL_KEY_0               48
#define CL_KEY_1               49
#define CL_KEY_2               50
#define CL_KEY_3               51
#define CL_KEY_4               52
#define CL_KEY_5               53
#define CL_KEY_6               54
#define CL_KEY_7               55
#define CL_KEY_8               56
#define CL_KEY_9               57
#define CL_KEY_SEMICOLON       59
#define CL_KEY_EQUAL           61
#define CL_KEY_A               65
#define CL_KEY_B               66
#define CL_KEY_C               67
#define CL_KEY_D               68
#define CL_KEY_E               69
#define CL_KEY_F               70
#define CL_KEY_G               71
#define CL_KEY_H               72
#define CL_KEY_I               73
#define CL_KEY_J               74
#define CL_KEY_K               75
#define CL_KEY_L               76
#define CL_KEY_M               77
#define CL_KEY_N               78
#define CL_KEY_O               79
#define CL_KEY_P               80
#define CL_KEY_Q               81
#define CL_KEY_R               82
#define CL_KEY_S               83
#define CL_KEY_T               84
#define CL_KEY_U               85
#define CL_KEY_V               86
#define CL_KEY_W               87
#define CL_KEY_X               88
#define CL_KEY_Y               89
#define CL_KEY_Z               90
#define CL_KEY_LEFT_BRACKET    91
#define CL_KEY_BACKSLASH       92
#define CL_KEY_RIGHT_BRACKET   93
#define CL_KEY_GRAVE_ACCENT    96
// Function keys
#define CL_KEY_ESCAPE          256
#define CL_KEY_ENTER           257
#define CL_KEY_TAB             258
#define CL_KEY_BACKSPACE       259
#define CL_KEY_INSERT          260
#define CL_KEY_DELETE          261
#define CL_KEY_RIGHT           262
#define CL_KEY_LEFT            263
#define CL_KEY_DOWN            264
#define CL_KEY_UP              265
#define CL_KEY_PAGE_UP         266
#define CL_KEY_PAGE_DOWN       267
#define CL_KEY_HOME            268
#define CL_KEY_END             269
#define CL_KEY_CAPS_LOCK       280
#define CL_KEY_SCROLL_LOCK     281
#define CL_KEY_NUM_LOCK        282
#define CL_KEY_PRINT_SCREEN    283
#define CL_KEY_PAUSE           284
#define CL_KEY_F1              290
#define CL_KEY_F2              291
#define CL_KEY_F3              292
#define CL_KEY_F4              293
#define CL_KEY_F5              294
#define CL_KEY_F6              295
#define CL_KEY_F7              296
#define CL_KEY_F8              297
#define CL_KEY_F9              298
#define CL_KEY_F10             299
#define CL_KEY_F11             300
#define CL_KEY_F12             301
#define CL_KEY_F13             302
#define CL_KEY_F14             303
#define CL_KEY_F15             304
#define CL_KEY_F16             305
#define CL_KEY_F17             306
#define CL_KEY_F18             307
#define CL_KEY_F19             308
#define CL_KEY_F20             309
#define CL_KEY_F21             310
#define CL_KEY_F22             311
#define CL_KEY_F23             312
#define CL_KEY_F24             313
#define CL_KEY_F25             314
#define CL_KEY_KP_0            320
#define CL_KEY_KP_1            321
#define CL_KEY_KP_2            322
#define CL_KEY_KP_3            323
#define CL_KEY_KP_4            324
#define CL_KEY_KP_5            325
#define CL_KEY_KP_6            326
#define CL_KEY_KP_7            327
#define CL_KEY_KP_8            328
#define CL_KEY_KP_9            329
#define CL_KEY_KP_DECIMAL      330
#define CL_KEY_KP_DIVIDE       331
#define CL_KEY_KP_MULTIPLY     332
#define CL_KEY_KP_SUBTRACT     333
#define CL_KEY_KP_ADD          334
#define CL_KEY_KP_ENTER        335
#define CL_KEY_KP_EQUAL        336
#define CL_KEY_LEFT_SHIFT      340
#define CL_KEY_LEFT_CONTROL    341
#define CL_KEY_LEFT_ALT        342
#define CL_KEY_LEFT_SUPER      343
#define CL_KEY_RIGHT_SHIFT     344
#define CL_KEY_RIGHT_CONTROL   345
#define CL_KEY_RIGHT_ALT       346
#define CL_KEY_RIGHT_SUPER     347
#define CL_KEY_MENU            348

// key modifiers
#define CL_MOD_SHIFT           0x0001
#define CL_MOD_CONTROL         0x0002 
#define CL_MOD_ALT             0x0004 
#define CL_MOD_SUPER           0x0008 
#define CL_MOD_CAPS_LOCK       0x0010 
#define CL_MOD_NUM_LOCK        0x0020 

// key actions
#define CL_KEY_RELEASE         0
#define CL_KEY_PRESS           1

// mouse buttons
#define CL_MOUSE_BUTTON_1       0
#define CL_MOUSE_BUTTON_2       1
#define CL_MOUSE_BUTTON_3       2
#define CL_MOUSE_BUTTON_4       3
#define CL_MOUSE_BUTTON_5       4
#define CL_MOUSE_BUTTON_6       5
#define CL_MOUSE_BUTTON_7       6
#define CL_MOUSE_BUTTON_8       7
#define CL_MOUSE_BUTTON_LAST    GLFW_MOUSE_BUTTON_8
#define CL_MOUSE_BUTTON_LEFT    GLFW_MOUSE_BUTTON_1
#define CL_MOUSE_BUTTON_RIGHT   GLFW_MOUSE_BUTTON_2
#define CL_MOUSE_BUTTON_MIDDLE  GLFW_MOUSE_BUTTON_3

// mouse actions
// i believe these are not separated from key actions
// in GLFW, but this doesn't hurt
#define CL_MOUSE_RELEASE        0
#define CL_MOUSE_PRESS          1





// --------------------
// Global State Definition
// --------------------

typedef struct _clGlobalState {
	GLuint default_shader_prog;

	clBool is_initialized;
	GLFWwindow* window_handle;

	int win_w;
	int win_h;

	clColorRGBAf current_bg_color;
} clGlobalState;

extern clGlobalState GLOBAL_STATE;



// --------------------
//     Rendering
// --------------------


struct _RenderObject;

// each render object should define its own render function
typedef void (*pRenderFn)(struct _RenderObject*);

// stores all information relevent for rendering an object
// with GL
typedef struct _RenderObject {
	u8 id;                  // unique ID given to each RO
	u32 shader_id;          // the id of the shader this object uses
	clColorRGBAf color;     // color it should be drawn as
	GLuint vao, vbo, ebo;   // GL objects
	f32* vertices;          // raw vertex data
	GLuint tex2d;           // id of a texture
	pRenderFn render_fn;    // function describing how to render this object
} RenderObject;





// default number of items to initialze the render batch arrays
#define CL_RB_DEFAULT_SIZE 16

// stores a list of renderobjects
// and associated shader programs
typedef struct _RenderBatch {
	RenderObject* ros;
	u32 num_ros;
	u32 ros_cap; // capacity of array
	
	GLuint* shaders;
	u32 num_shaders;
	u32 shaders_cap; // capacity of array
} RenderBatch;




// global render batch
extern RenderBatch RENDER_BATCH;





// converts from (0,win_[w,h]) to (-1,1)
// assumes window coordinate's origin is at the top left
extern clPoint2f coordsWindowToGLFW(clPoint2u32 in);

// converts color from 0-255 to 0.0-1.0
extern clColorRGBf  colorRGBNormalize(clColorRGBu8 color);
extern clColorRGBAf colorRGBANormalize(clColorRGBAu8 color);





// --------------------
// GLFW Default Callbacks
// --------------------


// simply resizes the framebuffer according to a window change
extern void defaultFramebufferSizeCallback(GLFWwindow* window, int width, int height);
// simply prints the error message to STDERR
extern void defaultErrorCallback(int error, const char* description);
// closes the window on ESC press
extern void defaultKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);





// --------------------
// GLFW Initialization / CHlib internal funcs
//
// NOTE: `chgl` prefixed funcs are internal,
//          i.e. the user should not use them
// --------------------

// Initializes glfw in the normal way (with defaults) up until window creation
// and returns the created window.
extern GLFWwindow* chglInitGLFW(u32 width, u32 height, const char* title);
// Deinitializes glfw in the normal way and sets global states to defaults.
extern void chglDeinitGLFW(GLFWwindow* window);



// --------------------
//     Shaders
// --------------------

#define CL_DEF_SHDR_ID    0 // id given to the default shader


#define CL_GRID_SHDR_VS_PATH "./res/grid.vs"
#define CL_GRID_SHDR_FS_PATH "./res/grid.fs"


// Internal function to create default shader program.
extern GLuint chglCreateDefaultShaderProgram();
// Load a different shader from a file.
extern GLuint chglCreateShaderProgram(const u8* vs_data, const u8* fs_data);
// adds the shader program into the global state's list of shaders
// returns its ID
extern u32 chglAddShaderProgram(GLuint shader_prog);
// Internal function to delete a shader program.
extern void chglDeleteShaderProgram(GLuint shader_prog);



// ------------------------------
//     Render Batch Stuff
// ------------------------------


// initialize everything to zero/null
// by doing = { 0 }; this should work already,
// but it can't hurt to be safe
extern clError chglRenderBatchInit();
// frees all render batch memory
extern void    chglRenderBatchDeinit();

// adds a render object to the batch
extern void    chglAddRenderObject(RenderObject ro);

// render all batched render objects
// just calls the RO's render function
extern void    chglRenderBatchRenderAll();





// --------------------
// Final API
//
// NOTE: These are the only functions that are not camelCase
//       and are the ones the user is expceted to use
// --------------------

// initializes GLFW and some global state variables
// a valid rendering context/GL function pointers are set here
// essentially everything that is needed to do a draw call
extern clError InitCHlib(u32 win_w, u32 win_h, const char* title);
// renders all RenderObjects within the current RenderBatch
// as well as any other glfw loop-by-loop basis things
extern clError RenderAll();
// frees all memory and GLFW-related stuff
extern clError DeinitCHlib();

// Returns false (meaning the window should stay open)
// so long as nothing has set it to so so
extern clBool WindowShouldClose();
// closes window, basically
extern void SetWindowShouldClose();

// Clears the background with the given color
extern clError SetBackgroundColor(clColorRGBAu8 color);




// defines how to render the grid
extern void pGridRender(RenderObject* ro);

// initialies and displays a grid on the screen
// split up into w columns and h rows
// where the color in the i'th row and j'th column is given by
// colors[i*w + j]
// NOTE: assumes that the user calls this only once,
// and especially not in the render loop!
extern void MakeGrid(u32 w, u32 h, clColorRGBAu8* colors);





#define CL_RECT_VS_SHADER "./res/rect.vert"
#define CL_RECT_FS_SHADER "./res/rect.frag"
















#endif // _CHLIB_H
