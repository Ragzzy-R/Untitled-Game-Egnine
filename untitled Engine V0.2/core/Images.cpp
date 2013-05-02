#include "Images.h"
#include <iostream>


Images::Images(void)
{
}


/*=======================================================================
  									
  Description : this class loads and Images.Pretty much all formats are
				supported.As it is using SDL_image to load image.All 
				formats supported by SDL_image are supported.
  Parameters  : filename = the path of the image.
  created on  : 08/04/2013                                             
  Last Edited : 08/04/2013                                             
 =======================================================================*/
int Images::loadImage(const char* filename) {
	
    SDL_Surface* TextureImage; 
	if ((TextureImage = IMG_Load(filename)))  {
		int bpp = TextureImage->format->BytesPerPixel;
		imageWidth  = TextureImage->w;
		imageHeight = TextureImage->h;
		glGenTextures( 1, &m_handler );
		glBindTexture( GL_TEXTURE_2D, m_handler );

		int colorFormat = GL_RGB;

		if(bpp == 4)
			colorFormat = GL_RGBA;
		if(bpp == 1)
			colorFormat = GL_LUMINANCE;
		if(bpp == 2)
			colorFormat = GL_LUMINANCE_ALPHA;

		glTexImage2D(GL_TEXTURE_2D, 0, colorFormat,TextureImage->w, TextureImage->h, 0, colorFormat, GL_UNSIGNED_BYTE,TextureImage->pixels); 

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	}



    if (TextureImage)
	    SDL_FreeSurface(TextureImage);
	return 0;
}




/*=======================================================================
  									
  Description : this class renders and Images.Pretty much all formats are
				supported.As it is using SDL_image to load image.All 
				formats supported by SDL_image are supported.
  parameters  : x	  = the x co-ordinate of the  top left corner.
				y     = the y co-ordinate of the  top left corner.
				width = the width  to be drawn onto the screen.
				height= the height to be drawn onto the screen.

  created on  : 08/04/2013                                             
  Last Edited : 08/04/2013                                             
 =======================================================================*/
int Images::drawImage(float x,float y,int width,int height) {
	glBindTexture(GL_TEXTURE_2D, m_handler);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);

	glTexCoord2f( 0.0f, 1.0f ); glVertex3f( x, y+height, 0 );
	glTexCoord2f( 1.0f, 1.0f ); glVertex3f( x+width,y+height, 0 );
	glTexCoord2f( 1.0f, 0.0f ); glVertex3f( x+width,y, 0 );
	glTexCoord2f( 0.0f, 0.0f ); glVertex3f( x,y, 0 );

	glEnd();
	glDisable (GL_BLEND);
	return 0;
}




Images::~Images(void)
{
}



