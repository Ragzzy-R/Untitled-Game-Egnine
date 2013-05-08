#include "Animation.h"

Animation::Animation(void) {
	i = 0;
	frameDelay(50);
	isPaused = false;
	isAnimated = true;
	startTime = SDL_GetTicks();
}

int Animation::addframe(int x,int y,int w,int h) {
	AnimRect rect = {x,y,w,h};
	dimensionTable.push_back(rect);
	return 0;
}
int Animation::drawAnimation(int xpos,int ypos,int w,int h) {
	currentTime = SDL_GetTicks();
	timeElapsed = currentTime-startTime;
	int size;
	size = dimensionTable.size();
	if(i<size) {
		if(isAnimated) {
			
					drawfromSprite(dimensionTable[i].x,dimensionTable[i].y,dimensionTable[i].width,dimensionTable[i].height,xpos+(i*2),ypos,w,h);
					
		if(!isPaused) {
			if(timeElapsed >=delay) {
				i++;
				startTime = currentTime;
			}
			}
		}
	}
	else {
		i=0;
	}

   
	return 0;
}
void Animation::frameDelay(long msec) {
	 delay = msec;
}

void Animation::pause(bool flag) {
 isPaused = flag;
}
void Animation::stop() {
	isAnimated = false;
	i = 0;
}
void Animation::restart() {
	i = 0;
	isAnimated = true;
	isPaused = false;
}

int Animation::rotateAnimation(float angle) {
	rotateSprite(angle);
	return 0;
}
Animation::~Animation(void) {
}