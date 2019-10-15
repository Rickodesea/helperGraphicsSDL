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

demo_opengl_openwindow.c
12:15PM on October 12, 2019

authors:
	Alrick Grandison

© 2019, zlib
*/

#include <SDL2/SDL.h>
#include "glad.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 400

#define LIMIT (1000/60)

float _time = 0.0f, _fps = 0.0f;
unsigned int _vsync;
unsigned char* _frm;

void handleWindowEvent(SDL_Event event);
Uint32 print_timer (Uint32 interval, void *param);
void update_fps(unsigned int elapsed);

void physics();
void render();

int main() {

	//initialize SDL
	if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 ) {

		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return -1;
	}

	//Set OpenGL version
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	//Create Window via SDL
	SDL_Window* window = SDL_CreateWindow( 
		"helperOpenGLSDL OpenWindow", 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	if ( !window ) {

		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		SDL_Quit();
		return -1;
	}

	//Create Context
	SDL_GLContext context = SDL_GL_CreateContext( window );

	if ( !context ) {

		printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
		SDL_DestroyWindow( window );
		SDL_Quit();
		return -1;
	}

	//Make the Context current
	if ( SDL_GL_MakeCurrent( window, context ) < 0 ) {

		printf( "OpenGL context could not be made current! SDL Error: %s\n", SDL_GetError() );
		SDL_GL_DeleteContext( context );
		SDL_DestroyWindow( window );
		SDL_Quit();
		return -1;
	}

	unsigned int _vsync = 1;
	int enableSwapOK;

	RETRYSWAP:
	enableSwapOK = SDL_GL_SetSwapInterval( _vsync );

	if ( enableSwapOK < 0 ) {

		printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
		
		if ( _vsync == -1 ) {

			_vsync = 1;
			printf( "Attempting VSync Swap Interval: SWAP_INTERVAL(1).\n");
			goto RETRYSWAP;
		}

		_vsync = 0;
	}

	if ( !_vsync ) {

		printf( "Enabling manual framerate control: FPS_TARGET(60).\n");
	}

	_frm = (_vsync) ? "auto" : "manual";

	//Load OpenGL functions via GLAD
	if ( !gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) ) {

		printf( "Failed to initialized GLAD! GLAD Error: unable to load OpenGL functions\n" );
		SDL_GL_MakeCurrent( window, 0 );
		SDL_GL_DeleteContext( context );
		SDL_DestroyWindow( window );
		SDL_Quit();
		return -1;
	}

	//Set viewport dimension
	glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	//Set FPS Printer Callback
	SDL_TimerID print_timer_id = SDL_AddTimer(2500, print_timer, 0);

	SDL_Event event; memset(&event, 0, sizeof(SDL_Event));
	unsigned int running = 1;
	unsigned int lastTime = SDL_GetTicks();

	while ( running ) {
	
		while ( SDL_PollEvent( &event ) ) {

		    switch ( event.type ) {

		    	case SDL_QUIT: {

		    		running = 0;
		    	} break;

		    	case SDL_WINDOWEVENT: {

			        handleWindowEvent( event );
			    } break;
		    }
		}

		//Clear OpenGL Screen
		glClearColor ( 0.0f, 0.0f, 0.5f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		physics();
		render();

		//Update OpenGL double buffering
		SDL_GL_SwapWindow(window); //if _vsync == 1, then this will auto handle framerate

		//Capture Time
		unsigned int elapsed = lastTime;
		lastTime = SDL_GetTicks();
		elapsed = lastTime - elapsed;
		
		//Update FPS
		update_fps(elapsed);

		if ( !_vsync ) {
			//If _vsync == 0, then we will have to manually handle framerate

			unsigned int wait = ( elapsed < LIMIT ) ? LIMIT - elapsed : 0;
			update_fps ( wait );
			if ( wait ) SDL_Delay ( wait );
			lastTime += wait;
		}
	}

	//Remove FPS Print Timer
	SDL_RemoveTimer(print_timer_id);

	//remove the context
	SDL_GL_MakeCurrent( window, 0 );

	//delete
	SDL_GL_DeleteContext( context );
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}

void handleWindowEvent(SDL_Event event) {

	switch (event.window.event) {
		
		/*case SDL_WINDOWEVENT_SHOWN: {

	        SDL_Log("Window %d shown", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_HIDDEN: {

	        SDL_Log("Window %d hidden", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_EXPOSED: {

	        SDL_Log("Window %d exposed", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_MOVED: {

	        SDL_Log("Window %d moved to %d,%d",  event.window.windowID, event.window.data1, event.window.data2);
	    } break;*/

	    case SDL_WINDOWEVENT_RESIZED: {

	        SDL_Log("Window %d resized to %dx%d", event.window.windowID, event.window.data1, event.window.data2);
	        
	        //Update Viewport
	        glViewport( 0, 0, event.window.data1, event.window.data2 );
	    } break;

	   /*case SDL_WINDOWEVENT_SIZE_CHANGED: {

	        SDL_Log("Window %d size changed to %dx%d", event.window.windowID, event.window.data1, event.window.data2);
	    } break;

	    case SDL_WINDOWEVENT_MINIMIZED: {

	        SDL_Log("Window %d minimized", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_MAXIMIZED: {

	        SDL_Log("Window %d maximized", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_RESTORED: {

	        SDL_Log("Window %d restored", event.window.windowID);
	     } break;

	    case SDL_WINDOWEVENT_ENTER: {

	        SDL_Log("Mouse entered window %d", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_LEAVE: {

	        SDL_Log("Mouse left window %d", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_FOCUS_GAINED: {

	        SDL_Log("Window %d gained keyboard focus", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_FOCUS_LOST: {

	        SDL_Log("Window %d lost keyboard focus", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_CLOSE: {

	        SDL_Log("Window %d closed", event.window.windowID);
	    } break;

		#if SDL_VERSION_ATLEAST(2, 0, 5)
		////////////////////////////////////////////////////////////////////////////
	    case SDL_WINDOWEVENT_TAKE_FOCUS: {

	        SDL_Log("Window %d is offered a focus", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_HIT_TEST: {

	        SDL_Log("Window %d has a special hit test", event.window.windowID);
	    } break;
	    ////////////////////////////////////////////////////////////////////////////
		#endif

	    default: {

	        SDL_Log("Window %d got unknown event %d", event.window.windowID, event.window.event);
	    } break;*/
	}
}

Uint32 print_timer (Uint32 interval, void *param) {

	printf ( "%s> fps: %f, time: %f\n", _frm, _fps, _time );

	return interval;
}

void update_fps(unsigned int elapsed){

	_time = (float)elapsed / 1000.0f;
	_fps = 1.0f / _time ;
}

void physics(){}
void render(){}
