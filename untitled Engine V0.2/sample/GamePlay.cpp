#include "GamePlay.h"
#include"FontRenderer.h"
#include <iostream>


GamePlay::GamePlay(void)
{
}
void GamePlay::init(AppGameState *apg){
	GameState::init(apg);
	ss.loadImage("./tetris/THEME_1_BG_1.png");
	man.loadSprite("./tetris/daff.png");
	man.addframe(148,0,40,79);
	man.addframe(189,0,40,79);
	man.addframe(0,79,40,79);
	man.addframe(41,79,40,79);
	man.addframe(0,159,40,79);
	man.addframe(82,79,40,79);
	man.addframe(41,159,40,79);
	man.addframe(123,80,40,79);
	man.addframe(82,159,40,79);
	man.addframe(164,80,40,78);
	man.addframe(102,0,45,78);
	man.addframe(0,0,50,78);
	man.addframe(51,0,50,78);
	angle = 0;
	
	x=40;
}

void GamePlay::render() {
	ss.drawImage(0,0,m_apg->containerWidth,m_apg->containerHeight);
	man.drawAnimation(700,300,80,160);
	man.drawImage(300,300,200,200);
	fontRenderer.setColor(0xFF4040);
	fontRenderer.printWithShadow("Press 'P' to pause and R to resart daffy",24,55);
	fontRenderer.printWithShadow("Press 'S' to stop him",24,105);

	
	
	
}
void GamePlay::update() {
	userPressed.pollEvents();
	if(userPressed.isKeyDown(SDLK_SPACE)) {
		x+=5;
	}
	if(userPressed.IsKeyPressed(SDLK_RETURN)) {
		
		m_apg->changeState("MenuState");
	}
	if(userPressed.isKeyDown(SDLK_UP)) { 
		man.rotateAnimation(angle);
		man.rotateImage(angle);
		ss.rotateImage(-(angle));
		angle++;
	}
	if(userPressed.IsKeyPressed(SDLK_s)) 
		man.stop();

	if(userPressed.IsKeyPressed(SDLK_r))
		man.restart();
	if(userPressed.IsKeyPressed(SDLK_p)) {
		man.pause(true);
		
	}
	if(userPressed.IsKeyPressed(SDLK_ESCAPE)) {
		m_apg->doneWithGame = true;
	}
	}
 
GamePlay::~GamePlay(void)
{
}
