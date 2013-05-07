// COMMENTS WILL BE ADDED

#include "Common.h"
#include "FontRenderer.h"
#include <iostream>
#include <map>
// Global declaration for testing.. Will be handled by Engine
FontRenderer fontRenderer;

const wchar_t colorKey = L'§';

int colorList[16];
std::map<String, unsigned int> colorMap;

inline int next_p2(int a) {
	int rval = 1;
	while (rval < a)
		rval <<= 1; // or rval *= 2
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

// Creates a single font bitmap using Glyphs.. 16x16 total 256 chars with ASCII + extended ASCII
void FontRenderer::createFontTexture(TTF_Font* Font) {
	for (int ch = 0; ch < 255; ch++) {
		SDL_Color Color = { 255, 255, 255 };	// Set font color to white
		SDL_Surface *Message = TTF_RenderGlyph_Blended(
				const_cast<TTF_Font*>(Font), ch, Color); // Get a SDL surface, rendered a single character on it

		int width = charSize;
		int height = charSize;

		char* pixels = (char*) Message->pixels; // Pixel buffer from surface.. Since it is BlendedGlyph, it is 4 channels.
		int xCh = (ch % 16) * width; // Get characters starting point on grid.. x
		int yCh = (ch / 16) * height; // Get characters starting point on grid.. y
		int sOff = (yCh * textureWidth) + xCh; // Get characters starting pixel.. x

		for (int j = 0; j < height; j++) { // Loop for all pixels to fill characters area in total bitmap.
			for (int i = 0; i < width; i++) {
				int crOffset = (j * textureWidth + i);	// Current pixel
				int index = (i + Message->w * j)
						* Message->format->BytesPerPixel;

				fontTextureData[2 * (sOff + crOffset)] = 0xFF;// Alpha channel on bitmap is always 255
				fontTextureData[2 * (sOff + crOffset) + 1] =// Color channel on bitmap
						(i >= Message->w || j >= Message->h) ?
								0 : pixels[index + 3];
			}
		}

		int advance = 0;
		TTF_GlyphMetrics(Font, ch, &chars[ch].left, &chars[ch].cols,
				&chars[ch].top, &chars[ch].rows, &advance); // Get min-max points, advance etc.
		chars[ch].rows -= chars[ch].top;
		chars[ch].width = advance;
	}

	glGenTextures(1, &fontTextureID);	// create and get texture id from GPU
	glBindTexture(GL_TEXTURE_2D, fontTextureID);	// bind texture id
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);// set filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);// Clamp to borders..
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0,	// Send bitmap buffer to the GPU
			GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, fontTextureData);
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

	glTexCoord2d(sx, sy + cy);
	glVertex2d(xTranslate, yTranslate);

	glTexCoord2d(sx + cx, sy + cy);
	glVertex2d(xTranslate + chars[ch].cols, yTranslate);

	glTexCoord2d(sx + cx, sy);
	glVertex2d(xTranslate + chars[ch].cols, yTranslate - chars[ch].rows);

	glTexCoord2d(sx, sy);
	glVertex2d(xTranslate, yTranslate - chars[ch].rows);

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
		std::cout << "Unable to load font: " << fname << ", " << TTF_GetError()
				<< std::endl;
		return;
	}

	/*TTF_SetFontKerning(tmpfont, 1);
	 TTF_SetFontHinting(tmpfont, TTF_HINTING_NORMAL);*/

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

	baseColor = 0xFFFFFFFF;
}

void FontRenderer::registerColorNames() {
	colorMap["AliceBlue"] = 0xF0F8FF;
	colorMap["AntiqueWhite"] = 0xFAEBD7;
	colorMap["Aqua"] = 0x00FFFF;
	colorMap["Aquamarine"] = 0x7FFFD4;
	colorMap["Azure"] = 0xF0FFFF;
	colorMap["Beige"] = 0xF5F5DC;
	colorMap["Bisque"] = 0xFFE4C4;
	colorMap["Black"] = 0x000000;
	colorMap["BlanchedAlmond"] = 0xFFEBCD;
	colorMap["Blue"] = 0x0000FF;
	colorMap["BlueViolet"] = 0x8A2BE2;
	colorMap["Brown"] = 0xA52A2A;
	colorMap["BurlyWood"] = 0xDEB887;
	colorMap["CadetBlue"] = 0x5F9EA0;
	colorMap["Chartreuse"] = 0x7FFF00;
	colorMap["Chocolate"] = 0xD2691E;
	colorMap["Coral"] = 0xFF7F50;
	colorMap["CornflowerBlue"] = 0x6495ED;
	colorMap["Cornsilk"] = 0xFFF8DC;
	colorMap["Crimson"] = 0xDC143C;
	colorMap["Cyan"] = 0x00FFFF;
	colorMap["DarkBlue"] = 0x00008B;
	colorMap["DarkCyan"] = 0x008B8B;
	colorMap["DarkGoldenRod"] = 0xB8860B;
	colorMap["DarkGray"] = 0xA9A9A9;
	colorMap["DarkGreen"] = 0x006400;
	colorMap["DarkKhaki"] = 0xBDB76B;
	colorMap["DarkMagenta"] = 0x8B008B;
	colorMap["DarkOliveGreen"] = 0x556B2F;
	colorMap["Darkorange"] = 0xFF8C00;
	colorMap["DarkOrchid"] = 0x9932CC;
	colorMap["DarkRed"] = 0x8B0000;
	colorMap["DarkSalmon"] = 0xE9967A;
	colorMap["DarkSeaGreen"] = 0x8FBC8F;
	colorMap["DarkSlateBlue"] = 0x483D8B;
	colorMap["DarkSlateGray"] = 0x2F4F4F;
	colorMap["DarkTurquoise"] = 0x00CED1;
	colorMap["DarkViolet"] = 0x9400D3;
	colorMap["DeepPink"] = 0xFF1493;
	colorMap["DeepSkyBlue"] = 0x00BFFF;
	colorMap["DimGray"] = 0x696969;
	colorMap["DimGrey"] = 0x696969;
	colorMap["DodgerBlue"] = 0x1E90FF;
	colorMap["FireBrick"] = 0xB22222;
	colorMap["FloralWhite"] = 0xFFFAF0;
	colorMap["ForestGreen"] = 0x228B22;
	colorMap["Fuchsia"] = 0xFF00FF;
	colorMap["Gainsboro"] = 0xDCDCDC;
	colorMap["GhostWhite"] = 0xF8F8FF;
	colorMap["Gold"] = 0xFFD700;
	colorMap["GoldenRod"] = 0xDAA520;
	colorMap["Gray"] = 0x808080;
	colorMap["Green"] = 0x008000;
	colorMap["GreenYellow"] = 0xADFF2F;
	colorMap["HoneyDew"] = 0xF0FFF0;
	colorMap["HotPink"] = 0xFF69B4;
	colorMap["IndianRed "] = 0xCD5C5C;
	colorMap["Indigo "] = 0x4B0082;
	colorMap["Ivory"] = 0xFFFFF0;
	colorMap["Khaki"] = 0xF0E68C;
	colorMap["Lavender"] = 0xE6E6FA;
	colorMap["LavenderBlush"] = 0xFFF0F5;
	colorMap["LawnGreen"] = 0x7CFC00;
	colorMap["LemonChiffon"] = 0xFFFACD;
	colorMap["LightBlue"] = 0xADD8E6;
	colorMap["LightCoral"] = 0xF08080;
	colorMap["LightCyan"] = 0xE0FFFF;
	colorMap["LightGoldenRodYellow"] = 0xFAFAD2;
	colorMap["LightGray"] = 0xD3D3D3;
	colorMap["LightGreen"] = 0x90EE90;
	colorMap["LightPink"] = 0xFFB6C1;
	colorMap["LightSalmon"] = 0xFFA07A;
	colorMap["LightSeaGreen"] = 0x20B2AA;
	colorMap["LightSkyBlue"] = 0x87CEFA;
	colorMap["LightSlateGray"] = 0x778899;
	colorMap["LightSteelBlue"] = 0xB0C4DE;
	colorMap["LightYellow"] = 0xFFFFE0;
	colorMap["Lime"] = 0x00FF00;
	colorMap["LimeGreen"] = 0x32CD32;
	colorMap["Linen"] = 0xFAF0E6;
	colorMap["Magenta"] = 0xFF00FF;
	colorMap["Maroon"] = 0x800000;
	colorMap["MediumAquaMarine"] = 0x66CDAA;
	colorMap["MediumBlue"] = 0x0000CD;
	colorMap["MediumOrchid"] = 0xBA55D3;
	colorMap["MediumPurple"] = 0x9370DB;
	colorMap["MediumSeaGreen"] = 0x3CB371;
	colorMap["MediumSlateBlue"] = 0x7B68EE;
	colorMap["MediumSpringGreen"] = 0x00FA9A;
	colorMap["MediumTurquoise"] = 0x48D1CC;
	colorMap["MediumVioletRed"] = 0xC71585;
	colorMap["MidnightBlue"] = 0x191970;
	colorMap["MintCream"] = 0xF5FFFA;
	colorMap["MistyRose"] = 0xFFE4E1;
	colorMap["Moccasin"] = 0xFFE4B5;
	colorMap["NavajoWhite"] = 0xFFDEAD;
	colorMap["Navy"] = 0x000080;
	colorMap["OldLace"] = 0xFDF5E6;
	colorMap["Olive"] = 0x808000;
	colorMap["OliveDrab"] = 0x6B8E23;
	colorMap["Orange"] = 0xFFA500;
	colorMap["OrangeRed"] = 0xFF4500;
	colorMap["colorMapOrchid"] = 0xDA70D6;
	colorMap["PaleGoldenRod"] = 0xEEE8AA;
	colorMap["PaleGreen"] = 0x98FB98;
	colorMap["PaleTurquoise"] = 0xAFEEEE;
	colorMap["PaleVioletRed"] = 0xDB7093;
	colorMap["PapayaWhip"] = 0xFFEFD5;
	colorMap["PeachPuff"] = 0xFFDAB9;
	colorMap["Peru"] = 0xCD853F;
	colorMap["Pink"] = 0xFFC0CB;
	colorMap["Plum"] = 0xDDA0DD;
	colorMap["PowderBlue"] = 0xB0E0E6;
	colorMap["Purple"] = 0x800080;
	colorMap["Red"] = 0xFF0000;
	colorMap["RosyBrown"] = 0xBC8F8F;
	colorMap["RoyalBlue"] = 0x4169E1;
	colorMap["SaddleBrown"] = 0x8B4513;
	colorMap["Salmon"] = 0xFA8072;
	colorMap["SandyBrown"] = 0xF4A460;
	colorMap["SeaGreen"] = 0x2E8B57;
	colorMap["SeaShell"] = 0xFFF5EE;
	colorMap["Sienna"] = 0xA0522D;
	colorMap["Silver"] = 0xC0C0C0;
	colorMap["SkyBlue"] = 0x87CEEB;
	colorMap["SlateBlue"] = 0x6A5ACD;
	colorMap["SlateGray"] = 0x708090;
	colorMap["Snow"] = 0xFFFAFA;
	colorMap["SpringGreen"] = 0x00FF7F;
	colorMap["SteelBlue"] = 0x4682B4;
	colorMap["Tan"] = 0xD2B48C;
	colorMap["Teal"] = 0x008080;
	colorMap["Thistle"] = 0xD8BFD8;
	colorMap["Tomato"] = 0xFF6347;
	colorMap["Turquoise"] = 0x40E0D0;
	colorMap["Violet"] = 0xEE82EE;
	colorMap["Wheat"] = 0xF5DEB3;
	colorMap["White"] = 0xFFFFFF;
	colorMap["WhiteSmoke"] = 0xF5F5F5;
	colorMap["Yellow"] = 0xFFFF00;
	colorMap["YellowGreen"] = 0x9ACD32;
}

int FontRenderer::getFontHeight() {
	return h + (h * 0.618f / 2.0f);
}

int FontRenderer::getStringWidth(string str) {
	int len = 0;
	for (unsigned int l = 0; l < str.length(); l++)
		len += chars[(int) str[l]].width;

	return len;
}

void FontRenderer::setColor(String colorName) {
	std::map<String, unsigned int>::const_iterator iter = colorMap.find(
			colorName);
	if (iter != colorMap.end())
		baseColor = iter->second;
}

void FontRenderer::setColor(int colorHex) {
	baseColor = colorHex;
}

void FontRenderer::printWithShadow(string str, float x, float y) {
	print(str, x + 1, y + 1, true);
	print(str, x, y, false);
}

void FontRenderer::print(string str, float x, float y, bool shadow) {

	int color = baseColor;
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
	glPushAttrib(
			GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
	glMatrixMode (GL_MODELVIEW);
	glDisable (GL_LIGHTING);
	glEnable (GL_TEXTURE_2D);
	glDisable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(f, f1, f2, f3);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(x, y, 0);
	yTranslate = 0;
	xTranslate = 0;
				
	for (unsigned int k = 0; k < wstr.length(); k++) {
		std::cout << wstr.length() << " - " << k << std::endl;
		std::cout  << (int)wstr[k]  << " - " << (int)colorKey << std::endl;
		if (k + 1 < wstr.length() && wstr[k] == colorKey) {
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
			glBegin(GL_QUADS);
			renderChar(wstr[k]);
			glEnd();
		}

	}

	glPopMatrix();
	glPopAttrib();
}

