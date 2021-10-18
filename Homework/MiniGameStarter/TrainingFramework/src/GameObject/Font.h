#pragma once
#include"GameConfig.h"
#include <string>
#include "ft2build.h"
#include FT_FREETYPE_H


class Font
{

public:
	Font(const std::string& font);
	~Font();
	bool		Init(const std::string& font);
	GLuint		GetFontTextureId();
	GLuint		GetFontVboId();
	FT_Face		GetFace();
	FT_GlyphSlot GetGlyphSlot();

	void SetName(std::string name) {
		m_name = name;
	}
	std::string GetName() {
		return m_name;
	}
	void SetId(GLuint id) {
		m_id = id;
	}
	GLuint GetId() {
		return m_id;
	}

private:
	std::string		m_name;
	GLuint			m_id;
	GLuint			m_vboId;
	GLuint			m_textureId;

	FT_Library		m_library;
	FT_Face			m_face;
	FT_GlyphSlot	m_glyphSlot;
};
