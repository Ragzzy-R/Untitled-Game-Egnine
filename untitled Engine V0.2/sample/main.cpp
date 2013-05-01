#include"AppGameState.h"
#include"FontRenderer.h"
#include "GamePlay.h"
#include "MenuState.h"

int main(int argc, char* argv[]) {

	GamePlay gp;
	MenuState ms;
	AppGameState apg;
	apg.initializeEverything(800,600,false);
	apg.addState("MainState", &gp);
	apg.addState("MenuState", &ms);
	apg.changeState("MainState");
	fontRenderer.init("UpheavalPro.ttf", 22);
	//apg.currentState(1);
	return apg.start();

}
	
