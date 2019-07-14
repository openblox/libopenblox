/*
 * Copyright (C) 2019 John M. Harris, Jr. <johnmh@openblox.org>
 *
 * This file is part of OpenBlox.
 *
 * OpenBlox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenBlox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with OpenBlox. If not, see <https://www.gnu.org/licenses/>.
 */

#include "Font.h"

#if HAVE_IRRLICHT && HAVE_SDL2 && HAVE_FONTCONFIG

#include "OBException.h"
#include "OBRenderUtils.h"

#include <string.h>

#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace OB{
	int next_p2(int a){
		int rval = 1;
		while(rval < a){
			rval *= 2;
		}

		return rval;
	}
	
	void make_dlist(FT_Face face, char ch, GLuint list_base, GLuint* tex_base){
		if(FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT)){
			throw new OBException("FT_Load_Glyph failed");
		}

		FT_Glyph glyph;
		if(FT_Get_Glyph(face->glyph, &glyph)){
			throw new OBException("FT_Get_Glyph failed");
		}

		FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

		FT_Bitmap& bitmap = bitmap_glyph->bitmap;

		int w = next_p2(bitmap.width);
		int h = next_p2(bitmap.rows);

		GLubyte* expanded_data = new GLubyte[2 * w * h];

		for(int j = 0; j < h; j++){
			for(int i = 0; i < w; i++){
				expanded_data[2 * (i + j * w)] = expanded_data[2 * (i + j * w) + 1] = (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * j];
			}
		}

		glBindTexture(GL_TEXTURE_2D, tex_base[ch]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

		delete[] expanded_data;

		glNewList(list_base + ch, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, tex_base[ch]);

		glPushMatrix();

		glTranslatef(bitmap_glyph->left, 0, 0);
		glTranslatef(0, bitmap_glyph->top - bitmap.rows, 0);

		float x = (float)bitmap.width / (float)w;
	    float y = (float)bitmap.rows / (float)h;

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 0);
			glVertex2f(0, bitmap.rows);
			
			glTexCoord2f(0, y);
			glVertex2f(0, 0);

			glTexCoord2f(x, y);
			glVertex2f(bitmap.width, 0);

			glTexCoord2f(x, 0);
			glVertex2f(bitmap.width, bitmap.rows);
		}
		glEnd();

		glPopMatrix();

		glTranslatef(face->glyph->advance.x >> 6, 0, 0);

		glBitmap(0, 0, 0, 0, face->glyph->advance.x >> 6, 0, NULL);

		glEndList();
	}
	
	Font::Font(const char*  fileName, unsigned int h){
		textures = new GLuint[128];

	    height = h;

		FT_Library library;
		if(FT_Init_FreeType(&library)){
			throw new OBException("FT_Init_FreeType failed");
		}

		FT_Face face;

		if(FT_New_Face(library, fileName, 0, &face)){
			throw new OBException("FT_New_Face failed (probably couldn't access the font file)");
		}

		FT_Set_Char_Size(face, h << 6, h << 6, 96, 96);

		list_base = glGenLists(128);
		glGenTextures(128, textures);

		for(unsigned char i = 0; i < 128; i++){
			make_dlist(face, i, list_base, textures);
		}

		FT_Done_Face(face);

		FT_Done_FreeType(library);
	}

	Font::~Font(){
		glDeleteLists(list_base, 128);
		glDeleteTextures(128, textures);
		delete[] textures;
	}

	void Font::print(float x, float y, size_t maxlen, const char* fmt, ...){
		OBRenderUtils::pushScreenCoordinateMatrix();

	    float h = height / .63f;
		char* text = new char[maxlen];
		va_list ap;

		if(fmt == NULL){
			*text = 0;
		}else{
			va_start(ap, fmt);
			vsnprintf(text, maxlen, fmt, ap);
			va_end(ap);
		}

		std::string strText = std::string(text);
	    std::vector<std::string> lines;
		std::string::size_type pos = 0;
		std::string::size_type prev = 0;
		while((pos = strText.find('\n', prev)) != std::string::npos){
			lines.push_back(strText.substr(prev, pos - prev));
			prev = pos + 1;
		}
		lines.push_back(strText.substr(prev));

		glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT);
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glListBase(list_base);

		float modelviewMatrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);

		for(int i = 0; i < lines.size(); i++){
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(x, y - h * i, 0);
			glMultMatrixf(modelviewMatrix);

			glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());

			glPopMatrix();
		}

		glPopAttrib();

		OBRenderUtils::popProjectionMatrix();
	}
}

#endif
