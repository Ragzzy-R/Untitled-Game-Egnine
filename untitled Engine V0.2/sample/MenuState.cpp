#include "MenuState.h"
#include "AppGameState.h"
#include <iostream>
     
MenuState::MenuState(){
}

void MenuState::init(AppGameState *apg){
	GameState::init(apg);

	ss.loadImage("tetris/SPLASHES_SHEET_41024x600.png");
	//man.loadImage("man.png");
}

void MenuState::render() {


	ss.drawImage(0,0,m_apg->containerWidth,m_apg->containerHeight);
	fontRenderer.printWithShadow("PRESS ENTER TO CONTINUE",240,550, 0xFF4040);
	fontRenderer.printWithShadow(" ",16,90,0xFF1493);
	fontRenderer.printWithShadow("§ Untitled Engine §!", 260, 220, 0xADFF2F);


}
void MenuState::update() {
	userPressed.pollEvents();
	if(userPressed.IsKeyPressed(SDLK_RETURN)) {
		//x+=5;
		m_apg->changeState("MainState");
	}
}

MenuState::~MenuState(void)
{
}
