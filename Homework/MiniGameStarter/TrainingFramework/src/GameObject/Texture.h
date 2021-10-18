#pragma once
#include "utilities.h" 



class Texture
{
	GLuint		m_id; 
	GLuint		m_textureId;
	
public:
	GLint		Init(const char* fileName, GLuint wrapMode, GLuint minFilterMode, GLuint magFilterMode);
	void		SetTextureId(GLuint id);
	GLuint		Get2DTextureId();
	GLuint		Get2DTextureAdd();
};
