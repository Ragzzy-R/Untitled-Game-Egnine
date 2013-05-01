#include "GamePlay.h"
#include"FontRenderer.h"
#include <iostream>


GamePlay::GamePlay(void)
{
}
void GamePlay::init(AppGameState *apg){
	GameState::init(apg);
	//ss.loadImage("tetris/sprite.png");
	man.loadImage("ss.png");
	//x=40;
}

void GamePlay::render() {
	

	//ss.drawImage(0,0,m_apg->containerWidth,m_apg->containerHeight);
	//man.drawSubImage(0, 0, 640, 480, 0, 0);
	fontRenderer.printWithShadow("Hello there, my name is Özgür Eraslan. §7And this is §1Untitled Engine!", 16, 32, 0xFFFFFF);
	//map.drawImage(0,0,800,600);
	//man.drawImage(x,50,320,160);
	
}
void GamePlay::update() {
	userPressed.pollEvents();
	if(userPressed.isKeyDown(SDLK_SPACE)) {
		x+=5;
	}
	if(userPressed.IsKeyPressed(SDLK_c)) {
		
		m_apg->changeState("MenuState");
	}
	if(userPressed.IsKeyPressed(SDLK_ESCAPE)) {
		m_apg->doneWithGame = true;
	}
	}
 
GamePlay::~GamePlay(void)
{
}
