#pragma once

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdexcept>

using std::string;
using std::wstring;

// Comments will be added

class FontRenderer {
public:
	FontRenderer() {
	}

	FontRenderer(const char * fname, unsigned int h) {
		init(fname, h);
	}

	void init(const char * fname, unsigned int h);

	virtual ~FontRenderer() {
	}

	struct Characher {
		char ch;
		int cols;
		int rows;
		int left;
		int top;
		int width;
	};

	void print(string str, float x, float y, bool shadow = false);
	void printWithShadow(string str, float x, float y);

	int getStringWidth(string str);
	int getFontHeight();

	void setColor(String colorName);
	void setColor(int colorHex);
	
private:
	void renderChar(int c);
	void createFontTexture(TTF_Font* Font);
	void registerColorNames();

	unsigned char* fontTextureData;
	int charSize;
	int xTranslate;
	int yTranslate;
	float h;

	Characher chars[256];
	wstring allowedChars;

	int baseColor;

	unsigned int fontTextureID;
	int textureHeight;
	int textureWidth;
};

// Global declaration for testing.. Will be handled by Engine
extern FontRenderer fontRenderer;

