#include "MenuState.h"
#include "AppGameState.h"
#include <iostream>
     
MenuState::MenuState(){
}

void MenuState::init(AppGameState *apg){
	GameState::init(apg);

	ss.loadImage("tetris/menu.png");
	//man.loadImage("man.png");
}

void MenuState::render() {


	ss.drawImage(0,0,m_apg->containerWidth,m_apg->containerHeight);
	//map.drawImage(0,0,800,600);
	//man.drawImage(40,50,320,160);

}
void MenuState::update() {
	userPressed.pollEvents();
	if(userPressed.IsKeyPressed(SDLK_c)) {
		//x+=5;
		m_apg->changeState("MainState");
	}
}

MenuState::~MenuState(void)
{
}
