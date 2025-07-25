#include "CHlib/CHlib.h"
#include "CHlib/Utils.h"
#include "cglm/cglm.h"


clRectData rectBuffer[MAX_RECTANGLES];
u32 numBufferedRects;


void clInitRectangles()
{
	// points with black default color
	f32 vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f,  0.0f, 0.0f, 0.0f,
		0.5f, 0.5f,   0.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
	};
	
	u8 indices[] = {
		0, 1, 2,
		0, 3, 2,
	};

	mat4 identity = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
	};
	UNUSED(identity);

	GLuint vboVertices, vboTransforms, ebo, vao;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &vboTransforms);
	glBindBuffer(GL_ARRAY_BUFFER, vboTransforms);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

	
	// need to bind to the vertex buffer to do this bit
	
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5, (void*)(3 * (sizeof vertices[0])));

	// then to the transformation buffer for the rest
	glBindBuffer(GL_ARRAY_BUFFER, vboTransforms);
	glEnableVertexAttribArray(10);
	glVertexAttribPointer(10, 16, GL_FLOAT, GL_FALSE, 16, (void*)0);
	// set this one to instanced
	glVertexAttribDivisor(10, 1);

	// set the unit quad shiesse
	RESOURCE_MANAGER.rectVao = vao;
	RESOURCE_MANAGER.rectVboVertices = vboVertices;
	RESOURCE_MANAGER.rectVboTransforms = vboTransforms;
	RESOURCE_MANAGER.rectEbo = ebo;
}

CLBool clLoadRectangles(RenderObject *ro)
{
	UNUSED(ro);
	return false;
}

void pRectRender(RenderObject *ro)
{
	clRectContext* rectContext = (clRectContext*)ro->context;
	
	// send the transforms to the GPU
	glBindVertexArray(RESOURCE_MANAGER.rectVao);
    glBindBuffer(GL_VERTEX_ARRAY, RESOURCE_MANAGER.rectVboTransforms);
	glBufferSubData(GL_ARRAY_BUFFER, 0, rectContext->numTransforms * sizeof(mat4), rectContext->transforms);

	// i dont know how this will do
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RESOURCE_MANAGER.rectEbo);

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0, rectContext->numTransforms);
	
	return;
}

void pRectFree(RenderObject *ro)
{
    clRectContext* rectContext = (clRectContext*)ro->context;
	free(rectContext->transforms);
}

void DrawRectangle(u32 x, u32 y, u32 w, u32 h, CLColorRGBAu8 color)
{
	LOG_MESSAGE(LOG_ERROR, "DrawRectangle() NOT IMPLEMENTED.\n");
	UNUSED(x);
	UNUSED(y);
	UNUSED(w);
	UNUSED(h);
	UNUSED(color);
	return;
}
