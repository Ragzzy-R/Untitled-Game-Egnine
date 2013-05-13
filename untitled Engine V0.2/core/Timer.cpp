#include "Timer.h"
#include "Common.h"
#include <SDL_timer.h>

Timer::Timer(void)
{
	startTime = SDL_GetTicks();
}

bool Timer::UEDelay(long msec) {

	currentTime = SDL_GetTicks();
		if(currentTime - startTime > msec) { // 1000 milisec = 1 sec
		startTime = currentTime;
		return true;
		}
		else 
			return false;
}
Timer::~Timer(void)
{
}
