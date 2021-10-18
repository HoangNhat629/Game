#include "Texture.h"

int Texture::Init(const char* fileName, GLuint wrapMode, GLuint minFilterMode, GLuint magFilterMode)
{
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	LOGI("Load 2D Texture:%s\t\t", fileName);

	GLint h, w, bpp;
	char* imageData = LoadTGA(fileName, &w, &h, &bpp);
	if (imageData == 0)
	{
		LOGE("ERROR");
		return 0;
	}
	switch (bpp)
	{
	case 24:
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		break;
	case 32:
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		break;
	}

	if (imageData != nullptr) delete[] imageData;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilterMode);

	if ((minFilterMode == GL_LINEAR_MIPMAP_LINEAR) || (minFilterMode == GL_LINEAR_MIPMAP_NEAREST) 
		|| (minFilterMode == GL_NEAREST_MIPMAP_LINEAR) || (minFilterMode == GL_NEAREST_MIPMAP_NEAREST))
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	LOGI("Done.\n");
	return 0;
}

void Texture::SetTextureId(GLuint id)
{
	m_id = id;
}
GLuint Texture::Get2DTextureId()
{
	return m_id;
}
GLuint Texture::Get2DTextureAdd()
{
	return m_textureId;
}