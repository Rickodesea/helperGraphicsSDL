/*
Copyright (c) <'2019'> <'Alrick Grandison'>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.


Alrick Grandison
	Innovative Director, Algodal

email: alrickgrandison@algodal.com

demo_opengl_triangle.c
12:05AM on October 14, 2019

authors:
	Alrick Grandison

© 2019, zlib
*/

#include "helperopenglsdl.h"
#include "helperopengl.h"
#include <SDL2/SDL.h>
#include "glad.h" //compile from source to use a different glad version

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init ();
void render ();
void destroy ();

HELPEROPENGLOBJECT object;
unsigned int program;

int main () {

	HELPEROPENGLSDL data;
	memset(&data, 0, sizeof(HELPEROPENGLSDL)); //always clear data

	data.name = "SDL OPENGL TRIANGLE";
	data.flags = SDL_WINDOW_RESIZABLE;
	data.fnInit = init;
	data.fnRender = render;
	data.fnCleanup = destroy;

	return helperOpenGLSDL(data);
}

int init () {

	float vertices[] = {
	    // positions         // colors
	     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
	    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
	     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	}; 
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2
    };

	HELPERBUFFER ver = {
		.array = vertices,
		.size = sizeof(vertices)
	};

	HELPERBUFFER ind = {
		.array = indices,
		.size = sizeof(indices)
	};

	SDL_RWops *vertRW = SDL_RWFromFile("source/demo/vertex01.vertex.shader", "rb");
	SDL_RWops *fragRW = SDL_RWFromFile("source/demo/fragment01.fragment.shader", "rb");

	unsigned int vertSize = SDL_RWsize(vertRW);
	unsigned int fragSize = SDL_RWsize(fragRW);

	unsigned char vertBuf[vertSize + 1];
	unsigned char fragBuf[fragSize + 1];

	memset(vertBuf, 0, vertSize + 1);
	memset(fragBuf, 0, fragSize + 1);

	SDL_RWread(vertRW, vertBuf, vertSize, 1);
	SDL_RWread(fragRW, fragBuf, fragSize, 1);

    SDL_RWclose(vertRW);
    SDL_RWclose(fragRW);

	object = helperOpenGL_Create_POSCOL(ver, ind, GL_STATIC_DRAW);
	program = helperOpenGL_Create_Program(vertBuf, fragBuf);

	return (object.vao && program) - 1;
}

void render () {

	glUseProgram(program);
	glBindVertexArray(object.vao); 
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void destroy () {

	helperOpenGL_Destroy_Object(object);
}