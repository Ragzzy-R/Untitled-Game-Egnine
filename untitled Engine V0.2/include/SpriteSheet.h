#pragma once
#include "Images.h"
class SpriteSheet :
	public Images
{
public:
	SpriteSheet(void);
	int loadSprite(const char* fname);
	int drawfromSprite(int x,int y,int sw,int sh,int xpos,int ypos,int dw,int dh);
	~SpriteSheet(void);
};

