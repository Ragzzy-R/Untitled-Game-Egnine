
#pragma once
#include "GameState.h"
#include "AppGameState.h"
#include "Images.h"
#include "Input.h"
class GamePlay :
	public GameState
{
public:
	GamePlay(void);
	char* key;
	Images ss;
	Images man;
	int x;
	Input userPressed;
	void init(AppGameState* apg);
	void update();
	void render();
	~GamePlay(void);
};


