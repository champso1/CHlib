#include "CHlib.h"


#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>




// --------------------
// BASIC DEFINES / GENERIC UTILS
// --------------------

clPoint2f coordsWindowToGLFW(clPoint2u32 in, u32 win_w, u32 win_h) {
	clPoint2f vec = {
		.x = (float)in.x/(float)(win_w/2) - 1.0f,
		.y = (float)in.y/(float)(win_h/2) - 1.0f
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
	fprintf(stderr, "[ERROR] (GLFW - ID: %d) %s\n", description);
}

void defaultKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// all we want to do is close the window should we press ESC or Q:
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
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
        fprintf(stderr, "[ERROR] Could not create window.\n");
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


const static char* default_vs_source =
	"#version 430 core\n"
	"in vec2 v_in_pos;\n"
	"void main() {\n"
    "    gl_Position = vec4(v_in_pos, 0.0, 1.0);\n"
	"}";
const static char* default_fs_source =
	"#version 430 core\n"
	"uniform vec3 u_color;\n"
	"out vec4 out_color;\n"
	"void main() {\n"
	"    out_color = vec4(u_color, 1.0);\n"
	"}";

GLuint chglCreateDefaultShaderProgram() {
	GLuint success = 0;
    char infoLog[512] = "";

    const GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &default_vs_source, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        fprintf(stderr, "[ERROR] Vertex shader compilation failed.\n");
        exit(1);
    }

    const GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &default_fs_source, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        fprintf(stderr, "[ERROR] Fragment shader compilation failed.\n");
        exit(1);
    }

    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    glGetShaderiv(prog, GL_LINK_STATUS, &success);
    if (success == 0) {
        glGetProgramInfoLog(vs, 512, NULL, infoLog);
        fprintf(stderr, "[ERROR] Shader program linking failed.\n");
        exit(1);
    }
    
    glDeleteShader(vs);
    glDeleteShader(fs);

	return prog;
}

void chglDeleteDefaultShaderProgram(GLuint shader_prog) {
	glDeleteProgram(shader_prog);
}





clGlobalState GLOBAL_STATE = { 0 };





clError InitCHlib(u32 win_w, u32 win_h, const char* title) {
	GLOBAL_STATE.is_initialized = false;
	
	GLFWwindow* window = chglInitGLFW(win_w, win_h, title);
	if (window == NULL) {
		fprintf(stderr, "[ERROR] (CHGL) Could not initialize GLFW.\n");
		return CL_ERROR;
	}

	GLOBAL_STATE.window_handle = window;
    GLOBAL_STATE.win_w = win_w;
	GLOBAL_STATE.win_h = win_h;

	GLuint default_shader_prog = chglCreateDefaultShaderProgram();
	GLOBAL_STATE.default_shader_prog = default_shader_prog;
	glUseProgram(default_shader_prog);
	
	// set rest of global state variables to defaults
	GLOBAL_STATE.current_bg_color = colorRGBANormalize(CL_DEFAULT_BG_COLOR);

	chglRenderBatchInit();
	
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
		fprintf(stderr, "[ERROR] CHlib is not initialized.\n");
		return CL_ERROR;
	}

	chglDeleteDefaultShaderProgram(GLOBAL_STATE.default_shader_prog);
	chglDeinitGLFW(GLOBAL_STATE.window_handle);
	chglRenderBatchDeinit();
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







clBool IsKeyDown(u32 key) {
	return (glfwGetKey(GLOBAL_STATE.window_handle, key) == GLFW_PRESS);
}






RenderBatch RENDER_BATCH = { 0 };




clError chglRenderBatchInit() {
	// RENDER_BATCH = { 0 } takes care of this in principle
	// but just to be safe
	// it's only called once, anyways
	for (int i=0; i<16; i++) {
		RENDER_BATCH.shapes[i] = (Shape){ 0 };
	}
	RENDER_BATCH.num_shapes = 0;
	return CL_OKAY;
}


void chglRenderBatchDeinit() {
    for (int i=0; i<RENDER_BATCH.num_shapes; i++) {
		RenderObject ro = RENDER_BATCH.shapes[i].ro;
		free(ro.vertices);
		free(ro.elements);
	}
}




RenderObject chglCreateRenderObject(f32* vertices, u32* elements, clColorRGBAf color) {
	RenderObject ro = { 0 };
	GLuint vao, vbo, ebo;
	
	const GLint in_pos_loc = glGetAttribLocation(GLOBAL_STATE.default_shader_prog, "v_in_pos");
	const GLint u_color_loc = glGetUniformLocation(GLOBAL_STATE.default_shader_prog, "u_color");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * (sizeof *vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * (sizeof *elements), elements, GL_STATIC_DRAW);
   
	glEnableVertexAttribArray(in_pos_loc);
	glVertexAttribPointer(in_pos_loc, 2, GL_FLOAT, GL_FALSE, 2 * (sizeof *vertices), (void*)0);
	

	ro.vao = vao;
	ro.vbo = vbo;
	ro.ebo = ebo;
	
	ro.color = color;
	ro.color_uniform_loc = u_color_loc;

	ro.vertices = malloc(8 * (sizeof *vertices));
	memcpy(ro.vertices, vertices, 8 * (sizeof *vertices));

	ro.elements = malloc(6 * (sizeof *elements));
	memcpy(ro.elements, elements, 6 * (sizeof *elements));

	return ro;
}




clError chglRenderBatchAddShape(Shape shape) {
	// just want to get this working
	// only allow 1 render object in here
	if (RENDER_BATCH.num_shapes >= 1) {
		return CL_OKAY;
	}
	RENDER_BATCH.shapes[RENDER_BATCH.num_shapes++] = shape;
	
	return CL_OKAY;
}




clError chglRenderBatchRenderAll() {
	for (u8 i=0; i<RENDER_BATCH.num_shapes; i++) {
		chglPrintShape(&RENDER_BATCH.shapes[i]);
		RenderObject ro = RENDER_BATCH.shapes[i].ro;

		glUseProgram(GLOBAL_STATE.default_shader_prog);
		
		glBindVertexArray(ro.vao);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ro.ebo);
		glUniform3f(ro.color_uniform_loc, ro.color.r, ro.color.g, ro.color.b);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	return CL_OKAY;
}



Shape chglMakeRect(RenderObject ro, f32 x, f32 y, f32 w, f32 h) {
	Shape shape = { 0 };
	shape.ro = ro;
	shape.shape_type = SHAPE_RECTANGLE;
	shape.x = x;
	shape.y = y;
	shape.w = w;
	shape.h = h;

	return shape;
}


void chglPrintShape(Shape shape) {
	if (shape.printed) return;
	
	printf("---------- SHAPE ----------\n");
	printf("VAO: %u, VBO: %u, EBO: %u\n", shape.ro.vao, shape.ro.vbo, shape.ro.ebo);
	printf("Type: ");
	if (shape.shape_type == SHAPE_RECTANGLE) {
		printf("Rectangle\n");
		printf("Pos: (%.2f, %.2f), Size: (%.2f, %.2f)\n", shape.x, shape.y, shape.w, shape.h);
	} else if (shape.shape_type == SHAPE_CIRCLE) {
		printf("Circle\n");
		printf("Pos: (%.2f, %.2f), Radius: %.2f\n", shape.x, shape.y, shape.r);
	}
	printf("Color: (r,g,b) -> (%.2f, %.2f, %.2f)\n", shape.ro.color.r, shape.ro.color.g, shape.ro.color.b);

	printf("Raw vertex data: [ ");
	for (int i=0; i<8; i++) {
		printf("%.2f ", shape.ro.vertices[i]);
	}
	printf("]\nRaw element data: [ ");
	for (int i=0; i<6; i++) {
		printf("%u ", shape.ro.elements[i]);
	}
	printf("]\n");
	
	
	printf("--------------------\n");
}


clError DrawRectangle(u32 x, u32 y, u32 w, u32 h, clColorRGBAu8 _color) {
	// bunch of normalizations!
	clPoint2f pos = coordsWindowToGLFW(
        (clPoint2u32){.x = x, .y = y},
		GLOBAL_STATE.win_w, GLOBAL_STATE.win_h
    );
	clVec2f size = {
		.w = (float)w/(float)GLOBAL_STATE.win_w,
		.h = (float)h/(float)GLOBAL_STATE.win_h
	};
	clColorRGBAf color = colorRGBANormalize(_color);
										
	return DrawRectangleVec(pos, size, color);
}




clError DrawRectangleVec(clPoint2f pos, clVec2f size, clColorRGBAf color) {
	
	f32 x = pos.x, y = pos.y;
	f32 w = size.w, h = size.h;
	f32 r = color.r, g = color.g, b = color.b;

	f32 vertices[] = {
	    x+w, y+h,
		x+w, y  ,
		x  , y  ,
		x  , y+h,
	};
	u32 elements[] = {
		0, 1, 3,
		1, 2, 3
	};
	
	RenderObject ro = chglCreateRenderObject(vertices, elements, color);
	Shape rect = chglMakeRect(ro, x, y, w, h);
	chglRenderBatchAddShape(rect);

	return CL_OKAY;
}
