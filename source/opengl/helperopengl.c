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

helperopengl.c
12:04AM on October 14, 2019

authors:
	Alrick Grandison

© 2019, zlib
*/

#include "helperopengl.h"
#include "glad.h" //assume that opengl functions already loaded

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERTEX \
"#version 330 core\n" \
"layout (location = 0) in vec3 aPos;\n" \
"void main()\n" \
"{\n" \
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" \
"}\0"

#define FRAGMENT \
"#version 330 core\n" \
"out vec4 FragColor;\n" \
"void main()\n" \
"{\n" \
"   FragColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);\n" \
"}\n\0"

#define BUFFERSIZE 1024

void helperOpenGL_Uniform_4f(unsigned int program, unsigned char* name, float r, float g, float b, float a) {

	unsigned int location = glGetUniformLocation(program, name);

	if ( location != -1 ) {

		glUseProgram(program);
		glUniform4f(location, r, g, b, a);
	} else {

		printf("Unable to find the uniform, %s\n", name);
	}
}

void helperOpenGL_Uniform_1i(unsigned int program, unsigned char* name, int i) {

	unsigned int location = glGetUniformLocation(program, name);

	if ( location != -1 ) {

		glUseProgram(program);
		glUniform1i(location, i);
	} else {

		printf("Unable to find the uniform, %s\n", name);
	}
}

void helperOpenGL_Uniform_1f(unsigned int program, unsigned char* name, float f) {

	unsigned int location = glGetUniformLocation(program, name);

	if ( location != -1 ) {

		glUseProgram(program);
		glUniform1i(location, f);
	} else {

		printf("Unable to find the uniform, %s\n", name);
	}
}

unsigned int helperOpenGL_Create_Program(unsigned char* vertex, unsigned char* fragment) {

	//default shaders are 330 core, you may want to change this when recompiling code
	vertex = (!vertex) ? VERTEX : (char*)vertex;
	fragment = (!fragment) ? FRAGMENT : (char*)fragment;

	unsigned int success = 0;
	unsigned char infoLog[BUFFERSIZE];

	//vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char*const*)&vertex, 0);
    glCompileShader(vertexShader);

    // check for shader compile errors
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {

        glGetShaderInfoLog(vertexShader, BUFFERSIZE, 0, infoLog);
        printf("Shader Compilation Error: Vertex: %s\n", infoLog);
        return 0;
    }

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char*const*)&fragment, 0);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, BUFFERSIZE, 0, infoLog);
        printf("Shader Compilation Error: Fragment: %s\n", infoLog);
        return 0;
    }

    // link shaders
    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {

        glGetProgramInfoLog(shaderProgram, BUFFERSIZE, 0, infoLog);
        printf("Shader Linking Error: Program: %s\n", infoLog);
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

HELPEROPENGLOBJECT helperOpenGL_Create_POS(HELPERBUFFER ver, HELPERBUFFER ind, unsigned int usuage) {

	const unsigned int unitsize = 3 * sizeof(float);

	HELPEROPENGLOBJECT obj;
	memset(&obj, 0, sizeof(HELPEROPENGLOBJECT));

	if ( !ver.size ) return obj;

	glGenVertexArrays(1, &obj.vao);
	glGenBuffers(1, &obj.vbo);
	if (ind.size) glGenBuffers(1, &obj.ebo);

	glBindVertexArray(obj.vao);

	glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
	glBufferData(GL_ARRAY_BUFFER, ver.size, ver.array, usuage);

	if (ind.size) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ebo);
    	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size, ind.array, usuage);
	}

	// position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, unitsize, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    return obj;
}

HELPEROPENGLOBJECT helperOpenGL_Create_POSCOL(HELPERBUFFER ver, HELPERBUFFER ind, unsigned int usuage) {

	const unsigned int unitsize = 6 * sizeof(float);

	HELPEROPENGLOBJECT obj;
	memset(&obj, 0, sizeof(HELPEROPENGLOBJECT));

	if ( !ver.size ) return obj;

	glGenVertexArrays(1, &obj.vao);
	glGenBuffers(1, &obj.vbo);
	if (ind.size) glGenBuffers(1, &obj.ebo);

	glBindVertexArray(obj.vao);

	glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
	glBufferData(GL_ARRAY_BUFFER, ver.size, ver.array, usuage);

	if (ind.size) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ebo);
    	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size, ind.array, usuage);
	}

	// position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, unitsize, (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, unitsize, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    return obj;
}

HELPEROPENGLOBJECT helperOpenGL_Create_POSCOLTEX(HELPERBUFFER ver, HELPERBUFFER ind, unsigned int usuage) {

	const unsigned int unitsize = 8 * sizeof(float);

	HELPEROPENGLOBJECT obj;
	memset(&obj, 0, sizeof(HELPEROPENGLOBJECT));

	if ( !ver.size ) return obj;

	glGenVertexArrays(1, &obj.vao);
	glGenBuffers(1, &obj.vbo);
	if (ind.size) glGenBuffers(1, &obj.ebo);

	glBindVertexArray(obj.vao);

	glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
	glBufferData(GL_ARRAY_BUFFER, ver.size, ver.array, usuage);

	if (ind.size) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ebo);
    	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size, ind.array, usuage);
	}

	// position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, unitsize, (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, unitsize, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, unitsize, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    return obj;
}

void helperOpenGL_Destroy_Object(HELPEROPENGLOBJECT obj) {

	if(obj.vao) {

		glDeleteVertexArrays(1, &obj.vao);
    	
    	if(obj.vbo) glDeleteBuffers(1, &obj.vbo);
    	if(obj.ebo) glDeleteBuffers(1, &obj.ebo);

    	memset(&obj, 0, sizeof(HELPEROPENGLOBJECT));
	}
}