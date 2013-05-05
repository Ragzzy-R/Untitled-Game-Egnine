/*=======================================================================
  Author      : Raghuram Iyer Razzy-R									
  Description : this class loads and renders Images.           
                
  created on  : 08/04/2013                                             
  Last Edited : 08/04/2013                                             
 =======================================================================*/
#include "AppGameState.h"
#pragma once
class Images
{
public:
	Images(void);
	GLuint m_handler;
	int    loadImage(const char* filename);

	int    drawImage(float x,float y,int width,int height);
	int imageWidth;
		SDL_Surface* TextureImage;
	int imageHeight;
	~Images(void);
};

