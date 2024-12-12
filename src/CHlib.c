#include "CHlib.h"


#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>




// --------------------
// BASIC DEFINES / GENERIC UTILS
// --------------------

chglVec2f coordsWindowToGLFW(chglVec2f in, int win_w, int win_h) {
	chglVec2f vec = {
		.x = (float)in.x/(float)(win_w/2) - 1.0f,
		.y = (float)in.y/(float)(win_h/2) - 1.0f
	};
	return vec;
}


chglColorf colorNormalize(chglColoru8 color) {
	return (chglColorf){
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






GLFWwindow* chglInitGLFW(int width, int height, const char* title) {
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
	"layout (location = 0) in vec2 in_pos;\n"
	"void main() {\n"
    "    gl_Position = vec4(in_pos, 0.0, 1.0);\n"
	"}";
const static char* default_fs_source =
	"#version 460 core\n"
	"out vec4 out_color;\n"
	"uniform vec3 in_color;\n"
	"void main() {\n"
	"    out_color = vec4(in_color, 1.0);\n"
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





chglState GLOBAL_STATE = { 0 };





chglError InitCHlib(int win_w, int win_h, const char* title) {
	GLOBAL_STATE.is_initialized = false;
	
	GLFWwindow* window = chglInitGLFW(win_w, win_h, title);
	if (window == NULL) {
		fprintf(stderr, "[ERROR] (CHGL) Could not initialize GLFW.\n");
		return CHGL_ERROR;
	}

	GLOBAL_STATE.window_handle = window;
    GLOBAL_STATE.win_w = win_w;
	GLOBAL_STATE.win_h = win_h;

	GLuint default_shader_prog = chglCreateDefaultShaderProgram();
	GLOBAL_STATE.default_shader_prog = default_shader_prog;
	glUseProgram(default_shader_prog);
	
	// set rest of global state variables to defaults
	GLOBAL_STATE.current_bg_color = colorNormalize(CHGL_DEFAULT_BG_COLOR);
	
	
	GLOBAL_STATE.is_initialized = true;
	return CHGL_OKAY;
}


chglError RenderAll() {
	glClearColor(
		 GLOBAL_STATE.current_bg_color.r,
		 GLOBAL_STATE.current_bg_color.g,
		 GLOBAL_STATE.current_bg_color.b,
		 GLOBAL_STATE.current_bg_color.a
	);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glfwSwapBuffers(GLOBAL_STATE.window_handle);
	glfwPollEvents();
	return CHGL_OKAY;
}


chglError DeinitCHlib() {
	if (!GLOBAL_STATE.is_initialized) {
		fprintf(stderr, "[ERROR] CHlib is not initialized.\n");
		return CHGL_ERROR;
	}

	chglDeleteDefaultShaderProgram(GLOBAL_STATE.default_shader_prog);
	chglDeinitGLFW(GLOBAL_STATE.window_handle);
	return CHGL_OKAY;
}



chglBool WindowShouldClose() {
	return glfwWindowShouldClose(GLOBAL_STATE.window_handle);
}

void SetWindowShouldClose() {
	glfwSetWindowShouldClose(GLOBAL_STATE.window_handle, GLFW_TRUE);
}


chglError SetBackgroundColor(chglColoru8 color) {
	chglColorf color_normed = colorNormalize(color);
	GLOBAL_STATE.current_bg_color = color_normed;
}




void SetCallbacks(chglCallbacks callbacks) {
	if (callbacks.framebuffer_size_callback != NULL) {
		glfwSetFramebufferSizeCallback(GLOBAL_STATE.window_handle, callbacks.framebuffer_size_callback);
	}
	if (callbacks.error_callback != NULL) {
		glfwSetErrorCallback(callbacks.error_callback);
	}
	if (callbacks.key_callback != NULL) {
		glfwSetKeyCallback(GLOBAL_STATE.window_handle, callbacks.key_callback);
	}
}

