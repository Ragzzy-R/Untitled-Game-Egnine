
#pragma once
#include "GameState.h"
#include "AppGameState.h"
#include "Input.h"
#include "Animation.h"
class GamePlay :
	public GameState
{
public:
	GamePlay(void);
	char* key;
	char* FPS;
	Images ss;
	Animation man;
	Animation man1;
	int x;
	Input userPressed;
	void init(AppGameState* apg);
	void update();
	void render();
	~GamePlay(void);
};


