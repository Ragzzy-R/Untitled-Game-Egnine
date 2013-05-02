

#include "AppGameState.h"
#include <iostream>

bool AppGameState::doneWithGame;
int AppGameState::containerHeight;
int AppGameState::containerWidth;
long AppGameState::FPS;

AppGameState::AppGameState(void)
{
}


/*=======================================================================
   Description : The Function adds the States to the StateManager.
				 Remember All states must be added via this method
				 in order to further do whatever with that state.once 
				 a state is added.its init() is called immedietly.So its
				 adviced not to do anything other than initialization in 
				 init()
   parameters  : 
		gs :is a pointer to the gamestate say gameplay state,mainmenustate
			etc.
		stateName: a string to keep track of the gamestate.	
   created on  : 25/04/2013                                             
   Last Edited : 27/04/2013                                             
 =======================================================================*/

void AppGameState::addState(String stateName, GameState *gs) {
	m_mapStates.insert(std::make_pair(stateName, gs));
	gs->init(this);
	currState = gs;
}

void AppGameState::changeState(String stateName) {
	GameStateMap::const_iterator iter = m_mapStates.find(stateName);
	if(iter != m_mapStates.end())
		currState = iter->second;
}

/*=======================================================================
   Description : this is the function which contains our game loop      
                 and triggers the loop.
   parameters  : NIL
   created on  : 07/04/2013                                             
   Last Edited : 27/04/2013                                             
 =======================================================================*/
int AppGameState::start() {
	startTime = SDL_GetTicks();
	frames=0;
	while(!doneWithGame) {
		currState->update();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		currState->render();

		frames++;

	    	//std::cout << "The start is :" << startTime << std::endl;
		currentTime = SDL_GetTicks();
		//std::cout<<"The current time is :" << currentTime << std::endl;
		if(currentTime - startTime > 1000) { // 1000 milisec = 1 sec
			FPS = frames; // Use this FPS value.

			frames = 0;
			startTime = currentTime;
		}
		
		SDL_GL_SwapBuffers();
		//std::cout << "The FPS is :" << FPS << std::endl;
		
	
	}
	
	return 0;
}
/*=======================================================================
   Description : this function initializes everything.It initializes SDL
				 openGL and creates a window.

   parameters  : Self-Explanatory
   created on  : 07/04/2013                                             
   Last Edited : 27/04/2013                                             
 =======================================================================*/
bool AppGameState::initializeEverything(int width,int height,bool fullscreenFlag) {
	
	 /* initialize SDL */
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
	  //MessageBox(NULL,(LPCWSTR)L"SDL initialization Failed:",(LPCWSTR)L"Untitled Engine",MB_ICONERROR | MB_OK);
	    exit(1);
	}
	SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,    	    8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,  	    8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,   	    8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,  	    8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  	    16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,		    32);

    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,	    8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,	8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,	    8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,	8);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);


	
    /* Fetch the video info */
    videoInfo = SDL_GetVideoInfo( );

    if ( !videoInfo ) {
	//MessageBox(NULL,(LPCWSTR)L"Unable to fetch Video Info;",(LPCWSTR)L"Untitled Engine",MB_ICONERROR | MB_OK);
	    exit(1);
	}

    /* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
    //videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */
	if(fullscreenFlag) 
		videoFlags |= SDL_FULLSCREEN;
    /* This checks to see if surfaces can be stored in memory */
    if ( videoInfo->hw_available )
	videoFlags |= SDL_HWSURFACE;
    else
	videoFlags |= SDL_SWSURFACE;

    /* This checks if hardware blits can be done */
    if ( videoInfo->blit_hw )

	videoFlags |= SDL_HWACCEL;

    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    if((surfDisplay = SDL_SetVideoMode(width,height,32,videoFlags)) == NULL) {
        return false;
    }
	
	/* Verify there is a surface */
    if ( !surfDisplay )
	{
	    //MessageBox(NULL,(LPCWSTR)L"Error creating a surface:",(LPCWSTR)L"Untitled Engine",MB_ICONERROR | MB_OK);
	    exit(1);
	}
	/* Enable smooth shading */
    glShadeModel( GL_SMOOTH );

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glViewport(0, 0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0,width,height, 0, 1, -1);

    glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glClearDepth( 1.0f );
	glDisable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glLoadIdentity();

	glEnable(GL_CULL_FACE);
	
	containerWidth=surfDisplay->w;
	containerHeight=surfDisplay->h;
return true;
}


AppGameState::~AppGameState(void)
{
}
	

