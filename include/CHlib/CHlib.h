#ifndef __CHLIB_H
#define __CHLIB_H


#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "CHlib/Utils.h"
#include "CHlib/HashMap.h"



// --------------------
//  Convenience Types
// --------------------
typedef enum _clError {CL_OKAY = 0, CL_ERROR = 1} CLError;
typedef enum _clBool  {false = 0, true = 1} CLBool;



// -------------------------
//   Vectors/Colors/Math
// ------------------------

/* Types follow an explicit naming convention:
 * 1) They are all prefixed with "CL" as a sort of C namespacing
 * 2) Vectors/Points are suffixed with the dimensionality and the type,
 *    and colors are suffixed with RGB or RGBA and the type
 * 
 * This makes it a bit cumbersome, but perfectly explicit.
 * If you get a CLPoint2u32, then you know exactly what it is used for and what it contains.
 */

struct _clVec2f
{
	union
	{
		struct { f32 x, y; };
		struct { f32 w, h; };
	};
};
typedef struct _clVec2f CLVec2f;
typedef struct _clVec2f CLPoint2f;


struct _clVec2u32
{
	union
	{
		struct { u32 x, y; };
		struct { u32 w, h; };
	};
};
typedef struct _clVec2u32 CLVec2u32;
typedef struct _clVec2u32 CLPoint2u32;


struct _clVec3f
{
	union
	{
		struct { f32 x, y, z; };
		struct { f32 r, g, b; };
	};
};
typedef struct _clVec3f CLVec3f;
typedef struct _clVec3f CLPoint3f;
typedef struct _clVec3f CLColorRGBf;


// not really feasible to define this as a point,
// since it only goes up to 255
struct _clVec3u8
{
	u8 r, g, b;
};
typedef struct _clVec3u8 CLVec3u8;
typedef struct _clVec3u8 CLColorRGBu8;


struct _clVec3u32
{
	u32 x, y, z;
};
typedef struct _clVec3u32 CLVec3u32;
typedef struct _clVec3u32 CLPoint3u32;


struct _clVec4u8
{
	u8 r, g, b, a;
};
typedef struct _clVec4u8 CLVec4u8;
typedef struct _clVec4u8 CLColorRGBAu8;


struct _clVec4f
{
	f32 r, g, b, a;
};
typedef struct _clVec4f CLVec4f;
typedef struct _clVec4f CLColorRGBAf;


// Some default colors:
#define CL_COLOR_BLACK       (CLColorRGBAu8){0, 0, 0, 0}
#define CL_COLOR_WHITE       (CLColorRGBAu8){0xFF, 0xFF, 0xFF, 0xFF}
#define CL_COLOR_BLUE        (CLColorRGBAu8){0x00, 0x00, 0xFF, 0xFF}
#define CL_COLOR_GREEN       (CLColorRGBAu8){0x00, 0xFF, 0x00, 0xFF}
#define CL_COLOR_RED         (CLColorRGBAu8){0xFF, 0x00, 0x00, 0xFF}
#define CL_COLOR_DARKGRAY    (CLColorRGBAu8){0x18, 0x18, 0x18, 0x18}

#define CL_DEFAULT_BG_COLOR  CL_COLOR_DARKGRAY




// used when comparing floats
#define CL_MIN_FLOAT_THRESHOLD 0.00001f







///! Converts from (0,win_[w,h]) to (-1,1)
///! NOTE: Assumes window coordinate's origin is at the top left
extern CLPoint2f coordsWindowToGLFW(CLPoint2u32 in);

///! Converts color from 0-255 to 0.0-1.0
///! Doesn't include alpha channel.
extern CLColorRGBf  colorRGBNormalize(CLColorRGBu8 color);

///! Converts color from 0-255 to 0.0-1.0
///! Does include alpha channel.
extern CLColorRGBAf colorRGBANormalize(CLColorRGBAu8 color);





// --------------------
//       INPUT
// --------------------

/* These are not used yet, really,
 * But I wanted to go ahead and stick them in.
 */

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




// ---------------------
//  Resource Management
// ---------------------

/* This resource manager idea is largely inspired from the
 * LearnOpenGL tutorial online. It's popular enough I won't
 * include any reference, just Google it.
 *
 * It does include some extras. Namely, at the moment,
 * it contains data related to unit quad
 * used for rendering rectangles.
 */

typedef struct _ResourceManager
{
	HashMap shaders;

	// unit quad data
	GLuint rectVao;
	GLuint rectVboVertices, rectVboTransforms;
	GLuint rectEbo;
} ResourceManager;


///! Initializes the data within the resource manager.
///! There is nothing to deinitialize; it's all on the stack!
extern ResourceManager clInitResourceManager();


// --------------------
//     Shaders
// --------------------

/* The way I do it here requires strict adherence to where shaders
 * are located, i.e. they cannot be stored anywhere else (yet).
 *
 * But by doing it this way, it makes API calls and such
 * Pretty simple to work with (in my opinion).
 * With storing the programs in a hashmap,
 * We can essentially reference everything with only the string.
 */


///! The path to where shaders are stored.
#define CL_SHADER_DIR "./res/Shaders/"


///! This is the final single call done from other functions.
///! Creates and stores the shader in the resource manager's
///! shader hashmap.
extern CLBool clLoadShader(str shader);

///! Abstracts away the OpenGL shader-related stuff,
///! especially the error checking.
///! Takes in the raw bytes and returns the shader id.
extern GLuint clCreateShaderProgram(const u8* vsData, const u8* fsData);

///! Deletes the given shader program.
///! Does nothing if the given shader program doesn't exist.
extern void clDeleteShaderProgram(str shader);

///! Grabs the shader id for the given shader
///! On error returns 0, since I think it's not a valid shader id
extern GLuint clGetShader(str shader);

///! Tells OpenGL to use the given shader program.
extern void clUseShader(str shader);






// --------------------
//     Rendering
// --------------------

// forward declare this bad boy
struct _RenderObject;

// each render object should define its own render function
// which is, as the name suggests, a way to define
// what to do to put it to the screen
typedef void (*pRenderFn)(struct _RenderObject*);

// each render object should also define how to
// free the additional memory it may need to allocate
typedef void (*pFreeFn)(struct _RenderObject*);

///! Stores all information relevent for
///! rendering an object with GL
typedef struct _RenderObject
{
	u8 id;                  // unique ID given to each RO (maybe not necessary)
	str shader;            // the shader this object uses
	CLColorRGBAf color;     // color it should be drawn as
	GLuint vao, vbo, ebo;   // GL objects
	f32* vertices;          // raw vertex data
	GLuint tex2d;           // id of a texture
	pRenderFn renderFn;     // function describing how to render this object
	pFreeFn freeFn;

	// additional data required 
	void* context;
} RenderObject;



// default number of items to initialze the render batch array(s)
#define CL_RB_DEFAULT_SIZE 16

///! Simply stores all of the render objects that are to be
///! drawn in the current render loop
///! it's just a dynamic array
typedef struct _RenderBatch
{
	RenderObject* ros;
	u32 num_ros;
	u32 ros_cap; // capacity of array
} RenderBatch;


///! Inializes and returns an empty render batch.
///! Array(s) are malloced to default sizes.
extern RenderBatch clRenderBatchInit();

///! Frees malloced data.
extern void clRenderBatchDeinit();

///! Adds a render object to the batch.
///! Handles resizing of the array
///! Returns false if it failed to add an object (i.e. it failed to reallocate the array)
extern CLBool clAddRenderObject(RenderObject ro);

///! Renders all of the objects in the batch.
///! Simply calls the associated RO's render function.
extern void clRenderBatchRenderAll();





// --------------------
// Global State
// --------------------

/* We have here a number of things that need to be kept
 * concurrent between API calls
 */

typedef struct _clGlobalState
{
	// window-related things
	u32 winW;
	u32 winH;
   	CLColorRGBAf currentBGColor;
	CLBool isInitialized;
	GLFWwindow* windowHandle;

	// other "Global" state handling things
	ResourceManager resourceManager;
	RenderBatch renderBatch;
} clGlobalState;

extern clGlobalState GLOBAL_STATE;

// to make it easier to reference the other global states
#define RENDER_BATCH      GLOBAL_STATE.renderBatch
#define RESOURCE_MANAGER  GLOBAL_STATE.resourceManager





// --------------------
// GLFW Initialization / CHlib internal funcs
// --------------------

///! Initializes glfw in the normal way (with defaults, largely)
///! up until window creation.
///! Returns the created window.
extern GLFWwindow* clInitGLFW(u32 width, u32 height, str title);

///! Deinitializes glfw and sets global states to defaults.
extern void clDeinitGLFW(GLFWwindow* window);





// --------------------
// Final API
//
// NOTE: These are the only functions that are not camelCase
//       and are the ones the user is expected to use
// --------------------

// initializes GLFW and some global state variables
// a valid rendering context/GL function pointers are set here
// essentially everything that is needed to do a draw call
extern CLError InitCHlib(u32 win_w, u32 win_h, const char* title);

// clears internal buffers and stuff
extern CLError StartRendering();

// renders all RenderObjects within the current RenderBatch
// as well as any other glfw loop-by-loop basis things
extern CLError RenderAll();

// frees all memory and GLFW-related stuff
extern CLError DeinitCHlib();

// Returns false (meaning the window should stay open)
// so long as nothing has set it to so so
extern CLBool WindowShouldClose();

// closes window, basically
extern void SetWindowShouldClose();

// Clears the background with the given color
extern void SetBackgroundColor(CLColorRGBAu8 color);




// defines how to render the grid
extern void pGridRender(RenderObject* ro);

// initialies and displays a grid on the screen
// split up into w columns and h rows
// where the color in the i'th row and j'th column is given by
// colors[i*w + j]
// NOTE: assumes that the user calls this only once,
// and especially not in the render loop!
extern CLError MakeGrid(u32 w, u32 h, CLColorRGBAu8* colors);






// cglm defines this later but this should work
typedef float mat4[4][4];

// the initial size of the rectangle transforms array
#define CL_INIT_RECT_TRANSFORMS 2



///! Internal struct to hold data for a rectangle;
typedef struct _clRectData
{
	// not necessarily necessary to store x, y, w, h
	// only the transform is actually used
	// but perhaps useful for debugging?
	u32 x, y, w, h;
	mat4 transform;
} clRectData;


///! Additional memory stored in a Render Object for rects
typedef struct _clRectContext
{
	mat4* transforms;
	u32 numTransforms;
} clRectContext;


// buffers rectangles before turning into an RO
#define MAX_RECTANGLES 16
extern clRectData rectBuffer[MAX_RECTANGLES];


///! Loads a quad and some initial data to the GPU
extern void clInitRectangles();

///! Turns all buffered rectangles into a single RO
///! Does this by accepting a pointer to the RO to fill
///! If there are no rectangles to load, just returns false
///! and does nothing with the memory, otherwise fills it
///! and returns true.
extern CLBool clLoadRectangles(RenderObject* ro);

///! Defines how to render a rectangle
extern void pRectRender(RenderObject* ro);

///! Defines how to free rect Render Object memory
extern void pRectFree(RenderObject* ro);

///! Final API call for the user to draw a rectangle.
extern void DrawRectangle(u32 x, u32 y, u32 w, u32 h, CLColorRGBAu8 color);



#endif // __CHLIB_H
