#include "CHlib/CHlib.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef _WIN32
	#include <direct.h>
	#define getCurrentDir _getcwd
#else
	#include <unistd.h>
	#define getCurrentDir getcwd
#endif



// --------------------
// BASIC DEFINES / GENERIC UTILS
// --------------------

CLPoint2f coordsWindowToGLFW(CLPoint2u32 in)
{
	CLPoint2f vec = {
		.x = (float)in.x/((float)GLOBAL_STATE.winW/2.0f) - 1.0f,
		.y = (float)in.y/((float)GLOBAL_STATE.winH/2.0f) - 1.0f
	};
	return vec;
}


CLColorRGBf colorRGBNormalize(CLColorRGBu8 color)
{
	return (CLColorRGBf){
		.r = (float)color.r / 255.0f,
		.g = (float)color.g / 255.0f,
		.b = (float)color.b / 255.0f
	};
}


CLColorRGBAf colorRGBANormalize(CLColorRGBAu8 color)
{
	return (CLColorRGBAf){
		.r = (float)color.r / 255.0f,
		.g = (float)color.g / 255.0f,
		.b = (float)color.b / 255.0f,
		.a = (float)color.a / 255.0f
	};
}




// --------------------
// GLFW Callbacks
// --------------------


_CL_INTERNAL void defaultFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	UNUSED(window);
	UNUSED(width);
	UNUSED(height);
	glViewport(0, 0, width, height);
}

_CL_INTERNAL void defaultErrorCallback(int error, const char* description)
{
	LOG_MESSAGE(LOG_ERROR, "(GLFW - ID: %d) %s\n", error, description);
}

_CL_INTERNAL void defaultKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	UNUSED(scancode);
	UNUSED(action);
	UNUSED(mods);
	// all we want to do is close the window should we press ESC or Q:
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}






GLFWwindow* clInitGLFW(u32 width, u32 height, str title)
{
	if (!glfwInit())
	{
		fprintf(stderr, "[ERROR] Could not initialize GLFW.\n");
		return null;
	}

	
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // make this shit float, damnit!!
	// NOTE: this is specifically so the window starts as "floating" on my linux machine
    //glfwWindowHintString(GLFW_X11_CLASS_NAME, "floating_class");
	//glfwWindowHintString(GLFW_X11_INSTANCE_NAME, "floating_instance");

    GLFWwindow* window = glfwCreateWindow(width, height, title, null, null);
    if (window == null)
	{
		LOG_MESSAGE(LOG_ERROR, "(clInitGLFW) Could not create window.\n");
        return null;
    }

    glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, defaultFramebufferSizeCallback);
	glfwSetErrorCallback(defaultErrorCallback);
	glfwSetKeyCallback(window, defaultKeyCallback);

	int glad_status = gladLoadGL();
	if (glad_status == 0)
	{
		LOG_MESSAGE(LOG_ERROR, "(clInitGLFW) Failed to initialize GLAD\n");
		return null;
	}
	LOG_MESSAGE(LOG_INFO, "(clInitGLFW) GLAD loaded with error code %d\n", glad_status);
	
	LOG_MESSAGE(LOG_INFO, "(clInitGLFW) Using OpenGL version: %s\n", glGetString(GL_VERSION));

	glfwSwapInterval(1);

	return window;
}




void clDeinitGLFW(GLFWwindow* window)
{
	glfwDestroyWindow(window);
	glfwTerminate();
}







ResourceManager clInitResourceManager()
{
	ResourceManager manager = { 0 };
	memset(&manager, 0, sizeof manager);
	
    manager.shaders = hmInit();

	/* TODO: add this back
	manager.rectTransforms = malloc(CL_INIT_RECT_TRANSFORMS * (sizeof *manager.rectTransforms));
	PTR_ASSERT(manager.rectTransforms, "(Resource Manager) Could not allocate memory for rectangle transforms.\n");

	manager.numRectTransforms = 0;
	manager.capRectTransforms = CL_INIT_RECT_TRANSFORMS;
	*/

	return manager;
}


_CL_INTERNAL void __cl_rm_resize_rect_transforms()
{
	/*
	RESOURCE_MANAGER.capRectTransforms *= 2;
	
	RESOURCE_MANAGER.rectTransforms = realloc(RESOURCE_MANAGER.rectTransforms, RESOURCE_MANAGER.capRectTransforms);
	PTR_ASSERT(RESOURCE_MANAGER.rectTransforms, "(Resource Manager) Could not reallocate new memory for rectangle transforms.\n");
	*/
}


void clAddRectTransform(mat4 transform)
{
	UNUSED(transform);
	/*
	if (RESOURCE_MANAGER.numRectTransforms == RESOURCE_MANAGER.capRectTransforms)
	{
		__cl_rm_resize_rect_transforms();
	}

	// I don't like not being able to do `sizeof transform`, but this shouldn't ever change,
	// so hopefully it will be okay
	memcpy(RESOURCE_MANAGER.rectTransforms[RESOURCE_MANAGER.numRectTransforms], transform, sizeof(mat4));
	RESOURCE_MANAGER.numRectTransforms += 1;
	*/
}



void clDeinitResourceManager()
{
	hmDeinit(RESOURCE_MANAGER.shaders);
	//free(RESOURCE_MANAGER.rectTransforms);
}




CLBool clLoadShader(str shader)
{
	// first read the data from the corresponding shader files
	
	// start with vertex shader
	char file_path[256];
	memset(file_path, 0, sizeof file_path);
	sprintf(file_path, "%s%s.vert", CL_SHADER_DIR, shader);

    u8* vert_buf = null;
	i32 bytes_read = readFile(file_path, &vert_buf);
	if (bytes_read < 0)
	{
		LOG_MESSAGE(LOG_ERROR, "Failed to load shader '%s' from file '%s'\n", shader, file_path);
		return false;
	}

	LOG_MESSAGE(LOG_INFO, "Read shader file %s (contained %d bytes)\n", file_path, bytes_read);

	// then the frag shader
	memset(file_path, 0, 256);
	sprintf(file_path, "%s%s.frag", CL_SHADER_DIR, shader);

	u8* fragBuf = null;
    bytes_read = readFile(file_path, &fragBuf);
	if (bytes_read < 0)
	{
		LOG_MESSAGE(LOG_ERROR, "Failed to load shader '%s' from file '%s'\n", shader, file_path);
		return false;
	}

	LOG_MESSAGE(LOG_INFO, "Read shader file %s (contained %d bytes)\n", file_path, bytes_read);


	GLuint shaderProg = clCreateShaderProgram(vert_buf, fragBuf);
	free(vert_buf);
	free(fragBuf);

	if(hmAdd(&RESOURCE_MANAGER.shaders, shader, (int)shaderProg) == 1)
	{
		LOG_MESSAGE(LOG_ERROR, "Key (shader) '%s' already exists.\n", shader);
		return false;
	}

	LOG_MESSAGE(LOG_INFO, "Loaded shader %s successfully.\n", shader);
	return true;
}



GLuint clCreateShaderProgram(const u8* vs_data, const u8* fs_data)
{
	GLint success = 0;
    char infoLog[512] = "";

    const GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	// this cast is so GCC shuts the eff up!
    glShaderSource(vs, 1, (const GLchar* const*)&vs_data, null);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (success == 0)
	{
        glGetShaderInfoLog(vs, 512, null, infoLog);
		LOG_MESSAGE(LOG_ERROR, "Vertex shader compilation failed.\n");
		LOG_MESSAGE(LOG_INFO, "Reason(s) for above error(s):\n%s", infoLog);
        exit(1);
    }

    const GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, (const GLchar* const*)&fs_data, null);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (success == 0)
	{
        glGetShaderInfoLog(fs, 512, null, infoLog);
		LOG_MESSAGE(LOG_ERROR, "Fragment shader compilation failed.\n");
		LOG_MESSAGE(LOG_INFO, "Reason(s) for above error(s):\n%s", infoLog);
        exit(1);
    }

    GLuint shader_prog = glCreateProgram();
    glAttachShader(shader_prog, vs);
    glAttachShader(shader_prog, fs);
    glLinkProgram(shader_prog);
    glGetShaderiv(shader_prog, GL_LINK_STATUS, &success);
    if (success == 0)
	{
        glGetProgramInfoLog(vs, 512, null, infoLog);
		LOG_MESSAGE(LOG_ERROR, "Shader program linking failed.\n");
		LOG_MESSAGE(LOG_INFO, "Reason(s) for above error(s):\n%s", infoLog);
        exit(1);
    }
	
    glDeleteShader(vs);
    glDeleteShader(fs);

	return shader_prog;
}

void clDeleteShaderProgram(str shader)
{
	HashTableEntry entry = hmGet(RESOURCE_MANAGER.shaders, shader);
	if (entry.type == HM_EMPTY)
		return;
	glDeleteProgram(entry.val);
}



GLuint clGetShader(str shader)
{
	HashTableEntry entry = hmGet(RESOURCE_MANAGER.shaders, shader);
	if (entry.type == HM_EMPTY)
	{
		LOG_MESSAGE(LOG_WARNING, "Hash table entry %s doesn't exist", shader);
		return 0;
	}

	return entry.val;
}



void clUseShader(str shader)
{
	HashTableEntry entry = hmGet(RESOURCE_MANAGER.shaders, shader);
	if (entry.type == HM_EMPTY)
	{
		LOG_MESSAGE(LOG_WARNING, "Shader %s has not been loaded.\n", shader);
		return;
	}

	glUseProgram(entry.val);
}






_CL_LOCAL_PERSIST u16 RO_ID;


RenderBatch clRenderBatchInit()
{
	RO_ID = 0;

	RenderBatch batch = { 0 };
	
    batch.ros = malloc(CL_RB_DEFAULT_SIZE * (sizeof *batch.ros));
	PTR_ASSERT(batch.ros, "[ERROR] Could not allocate memory for render batch render objects.\n");
	
	batch.ros_cap = CL_RB_DEFAULT_SIZE;
	batch.num_ros = 0;
	
	return batch;
}


void clRenderBatchDeinit()
{
	for (u32 i=0; i<RENDER_BATCH.num_ros; i++)
	{
		RenderObject* ro = &RENDER_BATCH.ros[i];
		if (ro->vertices != null)
			free(ro->vertices);
	}
	if (RENDER_BATCH.ros != null)
		free(RENDER_BATCH.ros);
}


CLBool clAddRenderObject(RenderObject ro)
{
	if (RENDER_BATCH.num_ros >= RENDER_BATCH.ros_cap)
	{
		RENDER_BATCH.ros_cap *= 2;
		RENDER_BATCH.ros = realloc(RENDER_BATCH.ros, RENDER_BATCH.ros_cap * (sizeof *RENDER_BATCH.ros));
		if (RENDER_BATCH.ros == null)
		{
			LOG_MESSAGE(LOG_ERROR, "(clAddRenderObject) Failed to reallocate RO array");
			return false;
		}
	}
	RENDER_BATCH.ros[RENDER_BATCH.num_ros] = ro;
	RENDER_BATCH.num_ros++;
	return true;
}



void clRenderBatchRenderAll()
{
	for (u8 i=0; i<RENDER_BATCH.num_ros; i++)
	{
		RenderObject* ro = &RENDER_BATCH.ros[i];
		ro->renderFn(ro);
	}
}



clGlobalState GLOBAL_STATE = { 0 };



CLError InitCHlib(u32 win_w, u32 win_h, str title)
{
	GLOBAL_STATE.isInitialized = false;

	char *cwd;
	if ((cwd = getCurrentDir(null, 0)) == null)
	{
		LOG_MESSAGE(LOG_ERROR, "(CHGL) Failed to retrieve the current directory.\n");
		return CL_ERROR;
	}
	LOG_MESSAGE(LOG_INFO, "(CHGL) Running from working directory: '%s'.\n", cwd);
	
	GLFWwindow* window = clInitGLFW(win_w, win_h, title);
	if (window == null)
	{
		LOG_MESSAGE(LOG_ERROR, "(CHGL) Could not initialize GLFW.\n");
		return CL_ERROR;
	}

	LOG_MESSAGE(LOG_INFO, "(CHGL) GLFW initialized.\n");

	GLOBAL_STATE.windowHandle = window;
    GLOBAL_STATE.winW = win_w;
	GLOBAL_STATE.winH = win_h;

	RENDER_BATCH = clRenderBatchInit();
	LOG_MESSAGE(LOG_INFO, "(CHGL) Render Batch initialized.\n");

	RESOURCE_MANAGER = clInitResourceManager();
	LOG_MESSAGE(LOG_INFO, "(CHGL) Resource manager initialized.\n");

	// load shaders
	if(!clLoadShader("default"))
	{
		LOG_MESSAGE(LOG_ERROR, "(CHGL) Failed to load 'default' shader\n");
		return CL_ERROR;
	}
	if (!clLoadShader("rect"))
	{
		LOG_MESSAGE(LOG_ERROR, "(CHGL) Failed to load 'rect' shader\n");
		return CL_ERROR;
	}
	
	// set rest of global state variables to defaults
	GLOBAL_STATE.currentBGColor = colorRGBANormalize(CL_DEFAULT_BG_COLOR);

	GLOBAL_STATE.isInitialized = true;
	LOG_MESSAGE(LOG_INFO, "(CHGL) CHGL initialized!\n");
	
	return CL_OKAY;
}



CLError DeinitCHlib()
{
	if (!GLOBAL_STATE.isInitialized)
	{
		LOG_MESSAGE(LOG_ERROR, "CHlib is not initialized.\n");
		return CL_ERROR;
	}

	clDeleteShaderProgram("default");
	clDeleteShaderProgram("rect");
	LOG_MESSAGE(LOG_INFO, "(DeinitCHlib) Default shader programs (default, rect) deleted.\n");
	
	clRenderBatchDeinit();
	LOG_MESSAGE(LOG_INFO, "(DeinitCHlib) Render batch deinitialized.\n");

	clDeinitResourceManager();
	LOG_MESSAGE(LOG_INFO, "(DeinitCHlib) Resource manager deinitialized.\n");

	clDeinitGLFW(GLOBAL_STATE.windowHandle);
	LOG_MESSAGE(LOG_INFO, "(DeinitCHlib) GLFW deinitialized!\n");
	
	return CL_OKAY;
}


CLError RenderAll()
{
	// load all rectangles
	RenderObject rectRo;
	if (clLoadRectangles(&rectRo))
		clAddRenderObject(rectRo);
	
	glClearColor(
	    GLOBAL_STATE.currentBGColor.r,
	    GLOBAL_STATE.currentBGColor.g,
	    GLOBAL_STATE.currentBGColor.b,
	    GLOBAL_STATE.currentBGColor.a
	);
	glClear(GL_COLOR_BUFFER_BIT);

    clRenderBatchRenderAll();
	
	glfwSwapBuffers(GLOBAL_STATE.windowHandle);
	glfwPollEvents();
	return CL_OKAY;
}



CLBool WindowShouldClose()
{
	return glfwWindowShouldClose(GLOBAL_STATE.windowHandle);
}

void SetWindowShouldClose()
{
	glfwSetWindowShouldClose(GLOBAL_STATE.windowHandle, GLFW_TRUE);
}


void SetBackgroundColor(CLColorRGBAu8 color)
{
	CLColorRGBAf color_normed = colorRGBANormalize(color);
	GLOBAL_STATE.currentBGColor = color_normed;
}






void pGridRender(RenderObject* ro)
{
	clUseShader(ro->shader);
	GLint loc = glGetUniformLocation(clGetShader(ro->shader), "tex_sampler");
	glUniform1i(loc, 0);
	glBindVertexArray(ro->vao);
	glActiveTexture(GL_TEXTURE0); // not necessary as only texture
	glBindTexture(GL_TEXTURE_2D, ro->tex2d);

	// nice substitute for EBO's!
	// but it might not work...
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}


CLError MakeGrid(u32 w, u32 h, CLColorRGBAu8* colors) {
	if(!clLoadShader("grid"))
	{
		LOG_MESSAGE(LOG_ERROR, "(MakeGrid) failed to load 'grid' shader.\n");
		return CL_ERROR;
	}
	clUseShader("grid");
	LOG_MESSAGE(LOG_INFO, "(MakeGrid) Grid shader loaded.\n");

	// we need to turn the colors into an array of bytes
	u8* tex_data = null;
	tex_data = malloc((w*h * 4) * (sizeof *tex_data));
	if (tex_data == null)
	{
		LOG_MESSAGE(LOG_ERROR, "(MakeGrid) failed to allocate memory for array of bytes for the color data.\n");
		return CL_ERROR;
	}
	for (u32 i=0, j=0; i<(w*h * 4); i+=4, j++)
	{
		tex_data[i] = colors[j].r;
		tex_data[i+1] = colors[j].g;
		tex_data[i+2] = colors[j].b;
		tex_data[i+3] = colors[j].a;
	}

	LOG_MESSAGE(LOG_INFO, "(MakeGrid) Pixel data is given by:\n");
	for (u32 i=0; i<w*h*4; i+=4)
		fprintf(stderr, "(%u,%u,%u,%u) ", tex_data[i], tex_data[i+1], tex_data[i+2], tex_data[i+3]);
	printf("\n");

	// now create a texture with it
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	// must do nearest so that it is actually a grid
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
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
	
	GLuint vao, vbo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	// use original array to get size nicely
	glBufferData(GL_ARRAY_BUFFER, sizeof _vertices, _vertices, GL_STATIC_DRAW);

	// screen coords
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * (sizeof *_vertices), (void*)0);

	// uv coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * (sizeof *_vertices), (void*)(2 * (sizeof *_vertices)));

	LOG_MESSAGE(LOG_INFO, "(MakeGrid) Vertex array object created / vertex data loaded.\n");

	// Make a Render Object with this data
	// and add it to the batch
	RenderObject ro = { 0 };
	ro.id = RO_ID++;
	ro.shader = "grid";
	ro.vao = vao;
	ro.vertices = malloc(sizeof _vertices);
	memcpy(ro.vertices, _vertices, sizeof _vertices);
	ro.tex2d = tex;
	ro.renderFn = pGridRender;

	if(!clAddRenderObject(ro))
	{
		LOG_MESSAGE(LOG_ERROR, "(MakeGrid) Failed to add grid RO to batch.\n");
		return CL_ERROR;
	}
	LOG_MESSAGE(LOG_INFO, "(MakeGrid) Grid RO added to batch.\n");
	return CL_OKAY;
}
