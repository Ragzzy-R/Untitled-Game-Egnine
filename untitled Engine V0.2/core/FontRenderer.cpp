
// COMMENTS WILL BE ADDED

#include "FontRenderer.h"
#include <iostream>

// Global declaration for testing.. Will be handled by Engine
FontRenderer fontRenderer;

const wchar_t colorKey = L'§';

int colorList[16];

inline int next_p2(int a) {
	int rval = 1;
	while (rval < a)
		rval *= 2;
	return rval;
}

void u8towstr(const std::string& src, std::wstring& dest) {
	dest.clear();
	wchar_t w = 0;
	int bytes = 0;
	wchar_t err = L'�';
	for (size_t i = 0; i < src.size(); i++) {
		unsigned char c = (unsigned char) src[i];
		if (c <= 0x7f) { //first byte
			if (bytes) {
				dest.push_back(err);
				bytes = 0;
			}
			dest.push_back((wchar_t) c);
		} else if (c <= 0xbf) { //second/third/etc byte
			if (bytes) {
				w = ((w << 6) | (c & 0x3f));
				bytes--;
				if (bytes == 0)
					dest.push_back(w);
			} else
				dest.push_back(err);
		} else if (c <= 0xdf) { //2byte sequence start
			bytes = 1;
			w = c & 0x1f;
		} else if (c <= 0xef) { //3byte sequence start
			bytes = 2;
			w = c & 0x0f;
		} else if (c <= 0xf7) { //4byte sequence start
			bytes = 3;
			w = c & 0x07;
		} else {
			dest.push_back(err);
			bytes = 0;
		}
	}
	if (bytes)
		dest.push_back(err);
}

void FontRenderer::createFontTexture(TTF_Font* Font) {
	for (int ch = 0; ch < 255; ch++) {
		SDL_Color Color = { 255, 255, 255 };
		SDL_Surface *Message = TTF_RenderGlyph_Blended(const_cast<TTF_Font*>(Font), ch, Color);

		int width = charSize;
		int height = charSize;
		char* pixels = (char*) Message->pixels;

		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				int xCh = (ch % 16) * width;
				int yCh = (ch / 16) * height;
				int sOff = (yCh * textureWidth) + xCh;
				int crOffset = (j * textureWidth + i);

				int index = (i + Message->w * j) * Message->format->BytesPerPixel;

				fontTextureData[2 * (sOff + crOffset)] = 0xFF;
				fontTextureData[2 * (sOff + crOffset) + 1] = (i >= Message->w || j >= Message->h) ? 0 : pixels[index + 3];
			}
		}

		int advance = 0;
		TTF_GlyphMetrics(Font, ch, &chars[ch].left, &chars[ch].cols, &chars[ch].top, &chars[ch].rows, &advance);
		chars[ch].rows -= chars[ch].top;
		chars[ch].width = advance;
	}

	glGenTextures(1, &fontTextureID);
	glBindTexture(GL_TEXTURE_2D, fontTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, fontTextureData);
}

void FontRenderer::renderChar(int ch) {
	int xCh = (ch % 16) * charSize;
	int yCh = (ch / 16) * charSize;
	float sx = (float) (xCh) / (float) textureWidth;
	float cx = (float) (chars[ch].cols) / (float) textureWidth;
	float sy = (float) (yCh) / (float) textureHeight;
	float cy = (float) (chars[ch].rows) / (float) textureHeight;
	xTranslate += chars[ch].left;
	yTranslate -= chars[ch].top;

	glBegin(GL_QUADS);
	glTexCoord2d(sx, sy + cy);
	glVertex2d(xTranslate, yTranslate);

	glTexCoord2d(sx + cx, sy + cy);
	glVertex2d(xTranslate + chars[ch].cols, yTranslate);

	glTexCoord2d(sx + cx, sy);
	glVertex2d(xTranslate + chars[ch].cols, yTranslate - chars[ch].rows);

	glTexCoord2d(sx, sy);
	glVertex2d(xTranslate, yTranslate - chars[ch].rows);

	glEnd();

	yTranslate += chars[ch].top;
	xTranslate += (chars[ch].width) - chars[ch].left;
}

void FontRenderer::init(const char * fname, unsigned int h) {
	this->h = h;

	//allowedChars = L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_'abcdefghijklmnopqrstuvwxyz{|}~⌂ÇüéâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜø£Ø×ƒáíóúñÑªº¿®¬½¼¡«»";

	TTF_Init();
	TTF_Font* tmpfont;
	tmpfont = TTF_OpenFont(fname, h);

	if (tmpfont == NULL) {
		printf("Unable to load font: %s %s \n", fname, TTF_GetError());
		// Handle the error here.
	}

	//TTF_SetFontHinting(tmpfont, TTF_HINTING_NORMAL);

	charSize = next_p2(h + 16);
	textureHeight = charSize * 16;
	textureWidth = charSize * 16;
	fontTextureData = new unsigned char[textureHeight * textureWidth * 2];
	memset(fontTextureData, 0x00, textureHeight * textureWidth * 2);

	createFontTexture(tmpfont);

	colorList[0] = 0x000000;
	colorList[1] = 0xFF0000;
	colorList[2] = 0x00FF00;
	colorList[3] = 0x0000FF;
	colorList[4] = 0xFFFF00;
	colorList[5] = 0xFF00FF;
	colorList[6] = 0x00FFFF;
	colorList[7] = 0xFF8000;
	colorList[8] = 0xFF0080;
	colorList[9] = 0x80FF00;
	colorList[10] = 0x00FF80;
	colorList[11] = 0x8000FF;
	colorList[12] = 0x0080FF;
	colorList[13] = 0x404040;
	colorList[14] = 0x808080;
	colorList[15] = 0xFFFFFF;
}

int FontRenderer::getFontHeight() {
	return h / 0.618f;
}

int FontRenderer::getStringWidth(string str) {
	int len = 0;
	for (unsigned int l = 0; l < str.length(); l++)
		len += chars[(int) str[l]].width;

	return len;
}

void FontRenderer::printWithShadow(string str, float x, float y, int color) {
	print(str, x + 1, y - 1, color, true);
	print(str, x, y, color, false);
}

void FontRenderer::print(string str, float x, float y, int color, bool shadow) {

	if (shadow)
		color = (color & 0xFCFCFC) >> 2;

	float f = (float) (color >> 16 & 0xFF) / 255.0;
	float f1 = (float) (color >> 8 & 0xFF) / 255.0;
	float f2 = (float) (color & 0xFF) / 255.0;
	float f3 = (float) (color >> 24 & 0xFF) / 255.0;
	if (f3 == 0.0) {
		f3 = 1.0;
	}

	wstring wstr;
	u8towstr(str, wstr);
	glBindTexture(GL_TEXTURE_2D, fontTextureID);

	// For testing .. Will be optimised
	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(f, f1, f2, f3);
	glPushMatrix();
	//glLoadIdentity();

	glTranslatef(x, y, 0);
	yTranslate = 0;
	xTranslate = 0;

	for (unsigned int k = 0; k < wstr.size(); k++) {
		if (k + 1 < wstr.size() && wstr[k] == colorKey) {
			int j1 = string("0123456789abcdef").find(tolower(wstr[k + 1]));
			if (j1 < 0 || j1 > 15) {
				j1 = 15;
			}

			int c = colorList[j1];
			if (shadow)
				c = (c & 0xFCFCFC) >> 2;

			glColor3f((float) (c >> 16) / 255.0, (float) (c >> 8 & 0xFF) / 255.0, (float) (c & 0xFF) / 255.0);

			k += 2;
		}

		if (k < wstr.length()) {
			renderChar(wstr[k]);
		}

	}

	glPopMatrix();
	glPopAttrib();
}

