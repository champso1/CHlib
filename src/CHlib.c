#include "CHlib.h"


#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "Utils.h"




// --------------------
// BASIC DEFINES / GENERIC UTILS
// --------------------

clPoint2f coordsWindowToGLFW(clPoint2u32 in) {
	clPoint2f vec = {
		.x = (float)in.x/(float)(GLOBAL_STATE.win_w/2) - 1.0f,
		.y = (float)in.y/(float)(GLOBAL_STATE.win_h/2) - 1.0f
	};
	return vec;
}


clColorRGBf colorRGBNormalize(clColorRGBu8 color) {
	return (clColorRGBf){
		.r = (float)color.r / 255.0f,
		.g = (float)color.g / 255.0f,
		.b = (float)color.b / 255.0f
	};
}


clColorRGBAf colorRGBANormalize(clColorRGBAu8 color) {
	return (clColorRGBAf){
		.r = (float)color.r / 255.0f,
		.g = (float)color.g / 255.0f,
		.b = (float)color.b / 255.0f,
		.a = (float)color.a / 255.0f
	};
}







// --------------------
// GLFW Callbacks
// --------------------


void defaultFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void defaultErrorCallback(int error, const char* description) {
	LOG_MESSAGE(LOG_ERROR, "(GLFW - ID: %d) %s\n", error, description);
}

void defaultKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// all we want to do is close the window should we press ESC or Q:
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}






GLFWwindow* chglInitGLFW(u32 width, u32 height, const char* title) {
	if (!glfwInit()) {
		fprintf(stderr, "[ERROR] Could not initialize GLFW.\n");
		return NULL;
	}

	
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //make this shit float, damnit!!
    glfwWindowHintString(GLFW_X11_CLASS_NAME, "floating_class");
    glfwWindowHintString(GLFW_X11_INSTANCE_NAME, "floating_instance");

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL) {
		LOG_MESSAGE(LOG_ERROR, "Could not create window.\n");
        return NULL;
    }


    glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, defaultFramebufferSizeCallback);
	glfwSetErrorCallback(defaultErrorCallback);
	glfwSetKeyCallback(window, defaultKeyCallback);

	gladLoadGL();
	
	return window;
}


void chglDeinitGLFW(GLFWwindow* window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}









GLuint chglCreateShaderProgram(const u8* vs_data, const u8* fs_data) {
	GLuint success = 0;
    char infoLog[512] = "";

    const GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	// this cast is so GCC shuts the eff up!
    glShaderSource(vs, 1, (const GLchar* const*)&vs_data, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
		LOG_MESSAGE(LOG_ERROR, "Vertex shader compilation failed.\n");
		LOG_MESSAGE(LOG_INFO, "Reason(s) for above error(s):\n%s", infoLog);
        exit(1);
    }

    const GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, (const GLchar* const*)&fs_data, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
		LOG_MESSAGE(LOG_ERROR, "Fragment shader compilation failed.\n");
		LOG_MESSAGE(LOG_INFO, "Reason(s) for above error(s):\n%s", infoLog);
        exit(1);
    }

    GLuint shader_prog = glCreateProgram();
    glAttachShader(shader_prog, vs);
    glAttachShader(shader_prog, fs);
    glLinkProgram(shader_prog);
    glGetShaderiv(shader_prog, GL_LINK_STATUS, &success);
    if (success == 0) {
        glGetProgramInfoLog(vs, 512, NULL, infoLog);
		LOG_MESSAGE(LOG_ERROR, "Shader program linking failed.\n");
		LOG_MESSAGE(LOG_INFO, "Reason(s) for above error(s):\n%s", infoLog);
        exit(1);
    }
    
    glDeleteShader(vs);
    glDeleteShader(fs);

	return shader_prog;
}







GLuint chglCreateDefaultShaderProgram() {
	const char* default_vs_data =
		"#version 430 core\n"
		"layout (location = 0) in vec2 v_in_pos;\n"
		"void main() {\n"
		"    gl_Position = vec4(v_in_pos, 0.0, 1.0);\n"
		"}";
	const char* default_fs_data =
		"#version 430 core\n"
		"layout (location = 1) uniform vec3 u_color;\n"
		"out vec4 out_color;\n"
		"void main() {\n"
		"    out_color = vec4(u_color, 1.0);\n"
		"}";

	GLuint shader_prog = chglCreateShaderProgram(default_vs_data, default_fs_data);
	return shader_prog;
}



u32 chglAddShaderProgram(GLuint shader_prog) {
	// double the size of the array if necessary
	if (RENDER_BATCH.num_shaders == RENDER_BATCH.shaders_cap) {
		RENDER_BATCH.shaders_cap *= 2;
		RENDER_BATCH.shaders = realloc(RENDER_BATCH.shaders, RENDER_BATCH.shaders_cap * (sizeof *RENDER_BATCH.shaders));
	}

	RENDER_BATCH.shaders[RENDER_BATCH.num_shaders] = shader_prog;
	u32 idx = RENDER_BATCH.num_shaders;
	RENDER_BATCH.num_shaders += 1;
	
    return idx;
}




void chglDeleteShaderProgram(GLuint shader_prog) {
	glDeleteProgram(shader_prog);
}










clGlobalState GLOBAL_STATE = { 0 };





clError InitCHlib(u32 win_w, u32 win_h, const char* title) {
	GLOBAL_STATE.is_initialized = false;
	
	GLFWwindow* window = chglInitGLFW(win_w, win_h, title);
	if (window == NULL) {
		LOG_MESSAGE(LOG_ERROR, "(CHGL) Could not initialize GLFW.\n");
		return CL_ERROR;
	}

	LOG_MESSAGE(LOG_INFO, "(CHGL) GLFW initialized.\n");

	GLOBAL_STATE.window_handle = window;
    GLOBAL_STATE.win_w = win_w;
	GLOBAL_STATE.win_h = win_h;

	GLuint default_shader_prog = chglCreateDefaultShaderProgram();
	GLOBAL_STATE.default_shader_prog = default_shader_prog;
	glUseProgram(default_shader_prog);

	LOG_MESSAGE(LOG_INFO, "(CHGL) Default shader program loaded.\n");
	
	// set rest of global state variables to defaults
	GLOBAL_STATE.current_bg_color = colorRGBANormalize(CL_DEFAULT_BG_COLOR);

	chglRenderBatchInit();
	LOG_MESSAGE(LOG_INFO, "(CHGL) Render Batch initialized.\n");
	
	GLOBAL_STATE.is_initialized = true;
	return CL_OKAY;
}


clError RenderAll() {
	glClearColor(
	    GLOBAL_STATE.current_bg_color.r,
	    GLOBAL_STATE.current_bg_color.g,
	    GLOBAL_STATE.current_bg_color.b,
	    GLOBAL_STATE.current_bg_color.a
	);
	glClear(GL_COLOR_BUFFER_BIT);

    chglRenderBatchRenderAll();
	
	glfwSwapBuffers(GLOBAL_STATE.window_handle);
	glfwPollEvents();
	return CL_OKAY;
}


clError DeinitCHlib() {
	if (!GLOBAL_STATE.is_initialized) {
		LOG_MESSAGE(LOG_ERROR, "CHlig is not initialized.\n");
		return CL_ERROR;
	}

	chglDeleteShaderProgram(GLOBAL_STATE.default_shader_prog);
	LOG_MESSAGE(LOG_INFO, "(DeinitCHlib) Default shader program deleted.\n");
	chglRenderBatchDeinit();
	LOG_MESSAGE(LOG_INFO, "(DeinitCHlib) Render batch deinitialized.\n");
	chglDeinitGLFW(GLOBAL_STATE.window_handle);
	LOG_MESSAGE(LOG_INFO, "(DeinitCHlib) GLFW deinitialized.\n");
	
	
	return CL_OKAY;
}



clBool WindowShouldClose() {
	return glfwWindowShouldClose(GLOBAL_STATE.window_handle);
}

void SetWindowShouldClose() {
	glfwSetWindowShouldClose(GLOBAL_STATE.window_handle, GLFW_TRUE);
}


clError SetBackgroundColor(clColorRGBAu8 color) {
	clColorRGBAf color_normed = colorRGBANormalize(color);
	GLOBAL_STATE.current_bg_color = color_normed;
	return CL_OKAY;
}



RenderBatch RENDER_BATCH = { 0 };
static u8 RO_ID;



clError chglRenderBatchInit() {
	RO_ID = 0;
	
    RENDER_BATCH.ros = malloc(CL_RB_DEFAULT_SIZE * (sizeof *RENDER_BATCH.ros));
	RENDER_BATCH.ros_cap = CL_RB_DEFAULT_SIZE;
	RENDER_BATCH.num_ros = 0;

	RENDER_BATCH.shaders = malloc(CL_RB_DEFAULT_SIZE * (sizeof *RENDER_BATCH.shaders));
	RENDER_BATCH.shaders_cap = CL_RB_DEFAULT_SIZE;
	RENDER_BATCH.num_shaders = 0;
	
	
	return CL_OKAY;
}


void chglRenderBatchDeinit() {

    for (int i=0; i<RENDER_BATCH.num_shaders; i++) {
		chglDeleteShaderProgram(RENDER_BATCH.shaders[i]);
	}
	free(RENDER_BATCH.shaders);

	for (int i=0; i<RENDER_BATCH.num_ros; i++) {
		RenderObject* ro = &RENDER_BATCH.ros[i];
		free(ro->vertices);
	}
	free(RENDER_BATCH.ros);
}


void chglAddRenderObject(RenderObject ro) {
	if (RENDER_BATCH.num_ros == RENDER_BATCH.ros_cap) {
		RENDER_BATCH.ros_cap *= 2;
		RENDER_BATCH.ros = realloc(RENDER_BATCH.ros, RENDER_BATCH.ros_cap * (sizeof *RENDER_BATCH.ros));
	}
	RENDER_BATCH.ros[RENDER_BATCH.num_ros] = ro;
	RENDER_BATCH.num_ros++;
}



void chglRenderBatchRenderAll() {
	for (u8 i=0; i<RENDER_BATCH.num_ros; i++) {
		RenderObject* ro = &RENDER_BATCH.ros[i];
		ro->render_fn(ro);
	}
}





void pGridRender(RenderObject* ro) {
	glUseProgram(RENDER_BATCH.shaders[ro->shader_id]);
	glBindVertexArray(ro->vao);
	// glActiveTexture(GL_TEXTURE0); // not necessary as only texture
	glBindTexture(GL_TEXTURE_2D, ro->tex2d);

	// nice substitute for EBO's!
	// but it might not work...
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}


void MakeGrid(u32 w, u32 h, clColorRGBAu8* colors) {
	// load in the grid shader stuff
	u8* vs_data = readFileData(CL_GRID_SHDR_VS_PATH);	
	u8* fs_data = readFileData(CL_GRID_SHDR_FS_PATH);
	if (vs_data == NULL || fs_data == NULL) {
		LOG_MESSAGE(LOG_ERROR, "(MakeGrid) Failed to load data from shader files.\n");
		return;
	}
	GLuint shader_prog = chglCreateShaderProgram(vs_data, fs_data);

	u32 shader_id = chglAddShaderProgram(shader_prog);

	LOG_MESSAGE(LOG_INFO, "(MakeGrid) Grid shader loaded.\n");

	// no longer need the raw file data
	free(vs_data);
	free(fs_data);

	// we need to turn the colors into an array of bytes
	u8* tex_data = NULL;
	tex_data = malloc((w*h * 4) * (sizeof *tex_data));
	for (int i=0, j=0; i<(w*h * 4); i+=4, j++) {
		tex_data[i] = colors[j].r;
		tex_data[i+1] = colors[j].g;
		tex_data[i+2] = colors[j].b;
		tex_data[i+3] = colors[j].a;
	}

	LOG_MESSAGE(LOG_INFO, "(MakeGrid) Pixel data is given by:\n");
	for (int i=0; i<w*h*4; i+=4) {
		fprintf(stderr, "(%u,%u,%u,%u) ", tex_data[i], tex_data[i+1], tex_data[i+2], tex_data[i+3]);
	}
	printf("\n");

	// now create a texture with it
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	// must do nearest so that it is actually a grid
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	LOG_MESSAGE(LOG_INFO, "(MakeGrid) Grid texture created and loaded.\n");
	
	// with this shader program and texture stuff
	// we just need to render a quad to the screen
	// need the GLFW screen coords for the quad itself
	// then the image/texture/UV coords
	// which are 0,0 at bottom left
	
	f32 _vertices[] = {
		// screen coords   // UV coords
		-1.0,  1.0,        0.0, 1.0,  // top-left
		 1.0,  1.0,        1.0, 1.0,   // top-right
		 1.0, -1.0,        1.0, 0.0,   // bottom-right
		-1.0, -1.0,        0.0, 0.0   // bottom-left
	};
	f32* vertices = malloc(sizeof _vertices);
	memcpy(vertices, _vertices, sizeof _vertices);
	
	GLuint vao, vbo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// here we have to use the original array rather than the malloced pointer to more accurately get the size
	glBufferData(GL_ARRAY_BUFFER, sizeof _vertices, _vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * (sizeof *vertices), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * (sizeof *vertices), (void*)(2 * (sizeof *vertices)));

	LOG_MESSAGE(LOG_INFO, "(MakeGrid) Vertex array object created / vertex data loaded.\n");



	// Make a Render Object with this data
	// and add it to the batch
	RenderObject ro = { 0 };
	ro.id = RO_ID++;
	ro.shader_id = shader_id;
	ro.vao = vao;
	ro.vertices = vertices;
	ro.tex2d = tex;
	ro.render_fn = pGridRender;

	chglAddRenderObject(ro);
	LOG_MESSAGE(LOG_INFO, "(MakeGrid) Grid RO added to batch.\n");
}




RenderObject rectBuffer[MAX_RECTBUFFER_SIZE] = { 0 };
u8 numRectangles = 0;


u32 chglInitRectangles() {
	u8* vs_data = readFileData(CL_RECT_VS_SHADER);	
	u8* fs_data = readFileData(CL_RECT_FS_SHADER);
	if (vs_data == NULL || fs_data == NULL) {
		LOG_MESSAGE(LOG_ERROR, "(MakeGrid) Failed to load rectangle shader files.\n");
		return 0;
	}
	GLuint shader_prog = chglCreateShaderProgram(vs_data, fs_data);
	u32 shader_idx = chglAddShaderProgram(shader_prog);

	return shader_idx;
}

void pRectRender(RenderObject* ro) {
	glUseProgram(RENDER_BATCH.shaders[ro->shader_id]);

	glBindVertexArray(ro->vao);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}



void DrawRectangle(u32 _x, u32 _y, u32 _w, u32 _h, clColorRGBAu8 _color) {
	// first check to make sure we aren't over count
	if (numRectangles >= MAX_RECTBUFFER_SIZE) {
		LOG_MESSAGE(LOG_WARNING, "(DrawRectangle) Too many rectangles in buffer. This rectangle will not be drawn.\n");
		return;
	}
	// if this is the first rectangle we need to initialize stuff
	u32 shader_idx;
	if (numRectangles == 0) {
		shader_idx = chglInitRectangles();
	} else {
		shader_idx = rectBuffer[0].shader_id;
	}

	clPoint2f pos = coordsWindowToGLFW((clPoint2u32){.x = _x, .y = _y});
	f32 x = pos.x, y = pos.y;
	clPoint2f size = (clPoint2f){
		.w = (f32)_w/(f32)GLOBAL_STATE.win_w,
		.h = (f32)_h/(f32)GLOBAL_STATE.win_h,
	};
	f32 w = size.w, h = size.h;

	clColorRGBAf color = colorRGBANormalize(_color);

	LOG_MESSAGE(LOG_WARNING, "(DrawRectangle) Rectangle attributes are:\n");
	fprintf(stderr, "Pos: (%.2f, %.2f),  Size: (%.2f, %.2f),  Color: (%.2f, %.2f, %.2f)\n", x, y, w, h, color.r, color.g, color.b);


	f32 _vertices[] = {
		x  , y  , color.r, color.g, color.b,
		x  , y+h, color.r, color.g, color.b,
		x+w, y+h, color.r, color.g, color.b,
		x+w, y  , color.r, color.g, color.b,
	};
	f32* vertices = NULL;
	// 4 indices, 2 coords, 3 color vals
	vertices = malloc(sizeof _vertices);
    memcpy(vertices, _vertices, sizeof _vertices);

	GLuint vao, vbo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof _vertices, _vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * (sizeof *vertices), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * (sizeof *vertices), (void*)(2 * (sizeof *vertices)));

	

	
	RenderObject ro = { 0 };
	ro.id = RO_ID++;
	ro.shader_id = shader_idx;
	ro.color = color;
    ro.vao = vao; ro.vbo = vbo;
	ro.vertices = vertices;
	ro.render_fn = pRectRender;

	chglAddRenderObject(ro);
	LOG_MESSAGE(LOG_INFO, "(DrawRectangle) Rectangle RO added to batch.\n");
	numRectangles++;
}
 
