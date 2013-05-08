#pragma once
#include "SpriteSheet.h"
#include <vector>
class Animation :
	public SpriteSheet
{
public:
	Animation(void);
	int i;
	long delay;
	int addframe(int x,int y,int width,int height);
	int drawAnimation(int xpos,int ypos,int w,int h);
	void pause(bool);
	void restart();
	void stop();
	void frameDelay(long);
	int  rotateAnimation(float angle);
	//int loadSprite(const char*);
		typedef struct {
		int x;
		int y;
		int width;
		int height;
	}AnimRect;
		~Animation(void);
private:
		std::vector<AnimRect> dimensionTable;
		bool isPaused;
		bool isAnimated;
		long currentTime;
		long startTime;
		long timeElapsed;
};