#pragma once
#include "images.h"
class SpriteSheet :
	public Images
{
public:
	SpriteSheet(void);
	int loadSprite(const char* fname);
	int drawfromSprite(float x,float y,float sw,float sh,float xpos,float ypos,float dw,float dh);
	~SpriteSheet(void);
};

