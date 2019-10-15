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

helperopengl.h
12:04AM on October 14, 2019

authors:
	Alrick Grandison

© 2019, zlib
*/

#ifndef _H_HELPEROPENGL
#define _H_HELPEROPENGL

typedef struct _HELPEROPENGLOBJECT {
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
} HELPEROPENGLOBJECT;

typedef struct _HELPERARRAY {
	void* array;
	unsigned int count;
} HELPERARRAY;

typedef struct _HELPERBUFFER {
	void* array;
	unsigned int size;
} HELPERBUFFER;

HELPEROPENGLOBJECT helperOpenGL_Create_POS(HELPERBUFFER ver, HELPERBUFFER ind, unsigned int usuage);
HELPEROPENGLOBJECT helperOpenGL_Create_POSCOL(HELPERBUFFER ver, HELPERBUFFER ind, unsigned int usuage);
HELPEROPENGLOBJECT helperOpenGL_Create_POSCOLTEX(HELPERBUFFER ver, HELPERBUFFER ind, unsigned int usuage);
unsigned int helperOpenGL_Create_Program(unsigned char* vertex, unsigned char* fragment);
void helperOpenGL_Destroy_Object(HELPEROPENGLOBJECT obj);
void helperOpenGL_Uniform_4f(unsigned int program, unsigned char* name, float r, float g, float b, float a);
void helperOpenGL_Uniform_1i(unsigned int program, unsigned char* name, int i);
void helperOpenGL_Uniform_1f(unsigned int program, unsigned char* name, float f);

#endif //_H_HELPEROPENGL
