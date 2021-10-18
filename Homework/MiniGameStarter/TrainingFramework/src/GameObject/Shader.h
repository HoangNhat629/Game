#pragma once
#include "utilities.h"

class Shader
{
public:
	Shader();
	~Shader();

	GLuint		m_program;
	GLint		m_iTextureLoc[5] = { 0 };

	GLint		Init(const std::string& fileVertexShader, const std::string& fileFragmentShader);
	void		EnableTest(GLuint gl_test);
	void		EnableAlpha();

	void		SetShaderID(GLuint id);
	GLuint		GetShaderID();
	GLuint		GetAttribLocation(const std::string& str);
	GLuint		GetUniformLocation(const std::string& str);

private:
	GLuint		m_shaderId;
	char		m_fileVS[260];
	char		m_fileFS[260];
	GLuint		m_vertexShaderId;
	GLuint		m_fragmentShaderId;
};