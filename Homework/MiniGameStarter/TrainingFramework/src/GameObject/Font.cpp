#include "Font.h"
#include "GameConfig.h"

Font::Font(const std::string& font)
{
	Init(font);
}

Font::~Font()
{
	FT_Done_Face(m_face);
	FT_Done_FreeType(m_library);
}

bool Font::Init(const std::string& font)
{
	std::string _font = font;

	if (FT_Init_FreeType(&m_library))
	{
		printf("Could not init freetype library\n");
		return false;
	}

	if (FT_New_Face(m_library, _font.c_str(), 0, &m_face)) {
		printf("Could not open font %s\n", font.c_str());
		return false;
	}
	FT_Set_Pixel_Sizes(m_face, 0, 48);
	m_glyphSlot = m_face->glyph;

	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenBuffers(1, &m_vboId);
	return true;
}


GLuint Font::GetFontTextureId()
{
	return m_textureId;
}

GLuint Font::GetFontVboId()
{
	return m_vboId;
}

FT_Face Font::GetFace() {
	return m_face;
}

FT_GlyphSlot Font::GetGlyphSlot() {
	return m_glyphSlot;
}