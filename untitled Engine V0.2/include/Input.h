#pragma once
#include "AppGameState.h"
class Input
{
public:
	Input(void);
	void initializeInputSystem();
	void pollEvents();
	void recordKeyPress();
	bool isKeyDown(SDLKey);
	bool IsKeyPressed(int pKey);
	bool IsKeyReleased(int pKey);
	~Input(void);
	private:
			  SDL_Event sEvent;
			  bool sPreviousKeyState[322];
			  bool sCurrentKeyState[322];
			  int sPreviousMouseX;
			  int sCurrentMouseX;
			  int sPreviousMouseY;
			  int sCurrentMouseY;
			  Uint8 sPreviousMouseState;
			  Uint8 sCurrentMouseState;
			  bool sWarpedMouse;
			  bool sScrollUp;
			  bool sScrollDown;
};

