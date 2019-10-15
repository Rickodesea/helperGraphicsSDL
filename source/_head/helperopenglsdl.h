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

helperopenglsdl.h
12:18PM on October 12, 2019

authors:
	Alrick Grandison

© 2019, zlib
*/

#ifndef _H_HELPEROPENGLSDL
#define _H_HELPEROPENGLSDL

#include <SDL2/SDL.h>

typedef struct _HELPEROPENGLSDL {

	void (*fnRender)(void);
	int (*fnInit)(void);
	void (*threadfnPhysics)(void);
	void (*userEvent)(SDL_Event);
	void (*userWindowEvent)(SDL_WindowEvent);
	void (*fnOnQuit)(void);
	void (*fnCleanup)(void);
	unsigned int width;
	unsigned int height;
	unsigned char* name;
	unsigned int flags;
	unsigned int glMajor;
	unsigned int glMinor;
	unsigned int printAllWindowStatus;
	unsigned int printFPS;
	unsigned int turnOffResizeStatus;
	unsigned int turnOffGLViewportUpdate;
} HELPEROPENGLSDL;

int helperOpenGLSDL(HELPEROPENGLSDL data);

#endif //_H_HELPEROPENGLSDL
