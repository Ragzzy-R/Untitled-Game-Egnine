#include "SpriteSheet.h"


SpriteSheet::SpriteSheet(void)
{
	rotAngle = 0;
}
int SpriteSheet::loadSprite(const char* filename) {
	
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

		glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, TextureImage->w, TextureImage->h, 0, colorFormat, GL_UNSIGNED_BYTE,TextureImage->pixels); 

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	}

    if (TextureImage)
	    SDL_FreeSurface(TextureImage);
	return 0;
}

int SpriteSheet::drawfromSprite(int x,int y,int sw,int sh,int xpos,int ypos,int dw,int dh) {

	float fx = 1.0 / (float)imageWidth;
	float fy = 1.0 / (float)imageHeight;
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	glTranslatef((xpos+(dw/2)),(ypos+(dh/2)),0.0f);
	glRotatef(rotAngle,0.0f,0.0f,1.0f);
	glTranslatef(-(xpos+(dw/2)),-(ypos+(dh/2)),0.0f);
	glBindTexture(GL_TEXTURE_2D, m_handler);
	glBegin(GL_QUADS);
	glTexCoord2f((float)(x + 00) * fx, (float)(y + sh) * fy); glVertex3f(xpos + 00, ypos + dh, 0.0f);
	glTexCoord2f((float)(x + sw) * fx, (float)(y + sh) * fy); glVertex3f(xpos + dw, ypos + dh, 0.0f);
	glTexCoord2f((float)(x + sw) * fx, (float)(y + 00) * fy); glVertex3f(xpos + dw, ypos + 00, 0.0f);
	glTexCoord2f((float)(x + 00) * fx, (float)(y + 00) * fy); glVertex3f(xpos + 00, ypos + 00, 0.0f);
	glEnd();
	glDisable(GL_BLEND); 
	glPopMatrix();
	return 0;
}

int SpriteSheet::rotateSprite(float angle) {
	rotAngle = angle;
	return 0;
}

SpriteSheet::~SpriteSheet(void)
{
}
