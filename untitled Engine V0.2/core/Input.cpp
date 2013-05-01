#include "Input.h"


Input::Input(void)
{
	initializeInputSystem();
}

void Input::initializeInputSystem() {
SDL_EnableKeyRepeat(0, 0);

		for(int i = 0; i < 322; i++)
		{
			sPreviousKeyState[i] = false;
			sCurrentKeyState[i] = false;
		}

		sPreviousMouseX = 0;
		sCurrentMouseX = 0;
		sPreviousMouseY = 0;
		sCurrentMouseY = 0;
		sPreviousMouseState = 0;
		sCurrentMouseState = 0;
	}

void Input::pollEvents() {

	for(int i = 0; i < 322; i++)
			sPreviousKeyState[i] = sCurrentKeyState[i];
		
		sScrollUp = false;
		sScrollDown = false;

		while(SDL_PollEvent(&sEvent))
		{
			switch(sEvent.type)
			{
				case SDL_KEYDOWN:
					sCurrentKeyState[sEvent.key.keysym.sym] = true;
					
					break;
				case SDL_KEYUP: 
					sCurrentKeyState[sEvent.key.keysym.sym] = false;

					break;
				/*case SDL_MOUSEBUTTONDOWN:
					if(sEvent.button.button == SDL_BUTTON_WHEELUP)
						sScrollUp = true;
					if(sEvent.button.button == SDL_BUTTON_WHEELDOWN)
						sScrollDown = true;
					break;*/
				case SDL_QUIT:
					AppGameState::doneWithGame= true;
					break;
			}
		}

		int resetX = 400;
		int resetY = 300;

		/*if(sWarpedMouse)
		{
			sPreviousMouseX = resetX;
			sPreviousMouseY = resetY;
			sWarpedMouse = false;
		}
		else
		{
			sPreviousMouseX = sCurrentMouseX;
			sPreviousMouseY = sCurrentMouseY;
		}*/

		//sPreviousMouseState = sCurrentMouseState;
		//sCurrentMouseState = SDL_GetMouseState(&sCurrentMouseX, &sCurrentMouseY);

		/* MOUSE WARPING
		if(sCurrentMouseX < 10 || sCurrentMouseX > 790 || sCurrentMouseY < 10 || sCurrentMouseY > 590)
		{
			//SDL_WarpMouse(resetX, resetY);
			sWarpedMouse = true;
		}
		*/
}
bool Input::isKeyDown(SDLKey key) {
	return sCurrentKeyState[key];
}

bool Input::IsKeyPressed(int pKey)
	{
		return sCurrentKeyState[pKey] && !sPreviousKeyState[pKey];
	}

	bool Input::IsKeyReleased(int pKey)
	{
		return !sCurrentKeyState[pKey] && sPreviousKeyState[pKey];
	}
Input::~Input(void)
{
}
