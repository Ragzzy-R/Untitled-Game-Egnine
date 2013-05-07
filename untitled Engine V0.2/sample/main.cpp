#include"AppGameState.h"
#include"FontRenderer.h"
#include "GamePlay.h"
#include "MenuState.h"

int main(int argc, char* argv[]) {

	GamePlay gp;
	MenuState ms;
	AppGameState apg;
	apg.initializeEverything("UntitledEngine V0.2",800,600,false);
	apg.addState("MainState", &gp);
	apg.addState("MenuState", &ms);
	apg.changeState("MenuState");
	fontRenderer.init("UpheavalPro.ttf", 16);
	return apg.start();

}

