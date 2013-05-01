

#pragma once
#include "GameState.h"
#include "Images.h"
#include "Input.h"
class MenuState :
	public GameState
{
public:
	MenuState(void);
	char key;
	Images ss;
	Images man;
	Input userPressed;
	void init(AppGameState* apg);
	void update();
	void render();
	~MenuState(void);
};
	
