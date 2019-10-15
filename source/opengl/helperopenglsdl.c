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

helperopenglsdl.c
12:18PM on October 12, 2019

authors:
	Alrick Grandison

© 2019, zlib
*/

#include "helperopenglsdl.h"
#include <SDL2/SDL.h>
#include "glad.h" //compile from source to use your own version of glad

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMIT (1000/60)

float _time = 0.0f, _fps = 0.0f;
unsigned int _vsync;
unsigned char* _frm;

void handleWindowEvent(HELPEROPENGLSDL data, SDL_Event event);
Uint32 print_timer (Uint32 interval, void *param);
void update_fps(unsigned int elapsed);

Uint32 physics_timer (Uint32 interval, void *param);
void dummyRender();
int dummyInit();
void dummyPhysics();
void dummyEvent(SDL_Event);
void dummyWindowEvent(SDL_WindowEvent);
void dummyQuit();
void dummyCleanup();
void setDefaults(HELPEROPENGLSDL* data);

int helperOpenGLSDL(HELPEROPENGLSDL data) {

	//Set Default Info
	setDefaults(&data);

	unsigned int SCREEN_WIDTH = data.width;
	unsigned int SCREEN_HEIGHT = data.height;

	//initialize SDL
	if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 ) {

		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return -1;
	}

	//Set OpenGL version
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, data.glMajor );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, data.glMinor );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	//Create Window via SDL
	SDL_Window* window = SDL_CreateWindow( 
		data.name, 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | data.flags
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

	unsigned int _vsync = 1; //best option - opengl auto targets 60 (mostly) FPS
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
	glViewport( 0, 0, data.width, data.height );

	//Set FPS Printer Callback
	SDL_TimerID print_timer_id = SDL_AddTimer(2500, print_timer, &data);

	//Physics
	SDL_TimerID physics_id = SDL_AddTimer(0004, physics_timer, &data);

	SDL_Event event; memset(&event, 0, sizeof(SDL_Event));
	unsigned int running = 1;
	unsigned int lastTime = SDL_GetTicks();

	//Initialization
	if ( data.fnInit() < 0 ) {

		printf( "User Initialization Function returned value less than 0.  Exiting Program.\n" );
		//Remove FPS Print Timer
		SDL_RemoveTimer(print_timer_id);

		//Remove Physics
		SDL_RemoveTimer(physics_id);

		//remove the context
		SDL_GL_MakeCurrent( window, 0 );

		//delete
		SDL_GL_DeleteContext( context );
		SDL_DestroyWindow( window );
		SDL_Quit();

		return -1;
	}

	while ( running ) {
	
		while ( SDL_PollEvent( &event ) ) {

		    switch ( event.type ) {

		    	case SDL_QUIT: {

		    		running = 0;
		    		data.fnOnQuit();
		    	} break;

		    	case SDL_WINDOWEVENT: {

			        handleWindowEvent( data, event );
			        data.userWindowEvent(event.window);
			    } break;

			    default: {

			    	data.userEvent(event);
			    } break;
		    }
		}

		//Clear OpenGL Screen
		glClearColor ( 0.0f, 0.0f, 0.5f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		data.fnRender();

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

	//Clean Up
	data.fnCleanup();

	//Remove FPS Print Timer
	SDL_RemoveTimer(print_timer_id);

	//Remove Physics
	SDL_RemoveTimer(physics_id);

	//remove the context
	SDL_GL_MakeCurrent( window, 0 );

	//delete
	SDL_GL_DeleteContext( context );
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}

void handleWindowEvent(HELPEROPENGLSDL data, SDL_Event event) {

	switch (event.window.event) {
		
		case SDL_WINDOWEVENT_SHOWN: {

	        if(data.printAllWindowStatus) SDL_Log("Window %d shown", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_HIDDEN: {

	        if(data.printAllWindowStatus) SDL_Log("Window %d hidden", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_EXPOSED: {

	        if(data.printAllWindowStatus) SDL_Log("Window %d exposed", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_MOVED: {

	        if(data.printAllWindowStatus) 
	        	SDL_Log("Window %d moved to %d,%d",  event.window.windowID, event.window.data1, event.window.data2);
	    } break;

	    case SDL_WINDOWEVENT_RESIZED: {

	        if(!data.turnOffResizeStatus) 
	        	SDL_Log("Window %d resized to %dx%d", event.window.windowID, event.window.data1, event.window.data2);
	        
	        //Update Viewport
	        if(!data.turnOffGLViewportUpdate)
	        	glViewport( 0, 0, event.window.data1, event.window.data2 );
	    } break;

	   	case SDL_WINDOWEVENT_SIZE_CHANGED: {

	        if(data.printAllWindowStatus) 
	        	SDL_Log("Window %d size changed to %dx%d", event.window.windowID, event.window.data1, event.window.data2);
	    } break;

	    case SDL_WINDOWEVENT_MINIMIZED: {

	        if(data.printAllWindowStatus) SDL_Log("Window %d minimized", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_MAXIMIZED: {

	        if(data.printAllWindowStatus) SDL_Log("Window %d maximized", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_RESTORED: {

	        if(data.printAllWindowStatus) SDL_Log("Window %d restored", event.window.windowID);
	     } break;

	    case SDL_WINDOWEVENT_ENTER: {

	        if(data.printAllWindowStatus) SDL_Log("Mouse entered window %d", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_LEAVE: {

	        if(data.printAllWindowStatus) SDL_Log("Mouse left window %d", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_FOCUS_GAINED: {

	        if(data.printAllWindowStatus) SDL_Log("Window %d gained keyboard focus", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_FOCUS_LOST: {

	        if(data.printAllWindowStatus) SDL_Log("Window %d lost keyboard focus", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_CLOSE: {

	        if(data.printAllWindowStatus) SDL_Log("Window %d closed", event.window.windowID);
	    } break;

		#if SDL_VERSION_ATLEAST(2, 0, 5)
		////////////////////////////////////////////////////////////////////////////
	    case SDL_WINDOWEVENT_TAKE_FOCUS: {

	        if(data.printAllWindowStatus) SDL_Log("Window %d is offered a focus", event.window.windowID);
	    } break;

	    case SDL_WINDOWEVENT_HIT_TEST: {

	        if(data.printAllWindowStatus) SDL_Log("Window %d has a special hit test", event.window.windowID);
	    } break;
	    ////////////////////////////////////////////////////////////////////////////
		#endif

	    default: {

	        if(data.printAllWindowStatus) 
	        	SDL_Log("Window %d got unknown event %d", event.window.windowID, event.window.event);
	    } break;
	}
}

Uint32 print_timer (Uint32 interval, void *param) {

	HELPEROPENGLSDL* data = (HELPEROPENGLSDL*)param;
	if (data->printFPS) printf ( "%s> fps: %f, time: %f\n", _frm, _fps, _time );

	return interval;
}

void update_fps(unsigned int elapsed){

	_time = (float)elapsed / 1000.0f;
	_fps = 1.0f / _time ;
}

void setDefaults(HELPEROPENGLSDL* data){

	if ( !data->glMajor ) {

		data->glMajor = 3;
		data->glMinor = 3;
	}

	if ( !data->name ) {

		data->name = "OpenGL SDL App";
	}

	if ( !data->width || !data->height ) {

		data->width = 640;
		data->height = 400;
	}

	if ( !data->fnRender ) {

		data->fnRender = dummyRender;
	}

	if ( !data->fnInit ) {

		data->fnInit = dummyInit;
	}

	if ( !data->threadfnPhysics ) {

		data->threadfnPhysics = dummyPhysics;
	}

	if ( !data->userEvent ) {

		data->userEvent = dummyEvent;
	}

	if ( !data->userWindowEvent ) {

		data->userWindowEvent = dummyWindowEvent;
	}

	if ( !data->fnOnQuit ) {

		data->fnOnQuit = dummyQuit;
	}

	if ( !data->fnCleanup ) {

		data->fnCleanup = dummyCleanup;
	}
}

Uint32 physics_timer (Uint32 interval, void *param) {

	HELPEROPENGLSDL* data = (HELPEROPENGLSDL*)param;
	data->threadfnPhysics();

	return interval;
}

void dummyRender(){}
int dummyInit(){ return 0; }
void dummyPhysics(){}
void dummyEvent(SDL_Event event){}
void dummyWindowEvent(SDL_WindowEvent windowEvent){}
void dummyQuit(){}
void dummyCleanup(){}
