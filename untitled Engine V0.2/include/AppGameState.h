/*=======================================================================
 * Author      : Raghuram Iyer Razzy-R									
 * Description : this is the class which initializes our Game           
 *               and triggers the loop.
 * created on  : 07/04/2013                                             
 * Last Edited : 27/04/2013                                             
 =======================================================================*/

#pragma once

#include "Common.h"
#include"GameState.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <map>



typedef std::map<String, GameState*> GameStateMap;

class AppGameState
{
public:
	AppGameState(void);
	GameState *gs;
	GameState *currState;
	SDL_Surface* surfDisplay;
	const SDL_VideoInfo *videoInfo;
	static bool doneWithGame;
	static int containerWidth;
	static int containerHeight;
	int videoFlags;
	long frames;
	long FPS;
	long startTime;
	long currentTime;
	bool resizeWindow(int,int,bool);
	void addState(String stateName, GameState*);
	void changeState(String stateName);
	void getDimension(int,int);
	int start();
	bool initializeEverything(const char* title,int width,int height,bool fullscreenFlag);
	GameStateMap m_mapStates;
	~AppGameState(void);
};

