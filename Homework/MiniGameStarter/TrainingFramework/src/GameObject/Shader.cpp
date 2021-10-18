#ifdef ANDROID
#include "../../Utilities/esShader.cpp"
#endif //android

#include "Shader.h"

Shader::Shader() : m_vertexShaderId(0), m_fragmentShaderId(0)
{

}

Shader::~Shader()
{
	glDeleteProgram(m_program);
	glDeleteShader(m_vertexShaderId);
	glDeleteShader(m_fragmentShaderId);
}

GLint Shader::Init(const std::string& fileVertexShader, const std::string& fileFragmentShader)
{
	LOGI("Compile Shader:%s\t%s\t\t\n", fileVertexShader.c_str(), fileFragmentShader.c_str());
	m_vertexShaderId = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);
	if (m_vertexShaderId == 0)
	{
		LOGE("ERR VS %s\n", fileVertexShader.c_str());
		return -1;
	}
	m_fragmentShaderId = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);
	if (m_fragmentShaderId == 0)
	{
		glDeleteShader(m_vertexShaderId);
		LOGE("ERR FS %s\n", fileFragmentShader.c_str());
		return -2;
	}

	m_program = esLoadProgram(m_vertexShaderId, m_fragmentShaderId);

	//finding location of uniforms / attributes
	m_iTextureLoc[0] = glGetUniformLocation(m_program, "u_texture0");
	m_iTextureLoc[1] = glGetUniformLocation(m_program, "u_texture1");
	m_iTextureLoc[2] = glGetUniformLocation(m_program, "u_texture2");
	m_iTextureLoc[3] = glGetUniformLocation(m_program, "u_texture3");
	m_iTextureLoc[4] = glGetUniformLocation(m_program, "u_texture4");
	LOGI("Done %s %s \n", fileVertexShader.c_str(), fileFragmentShader.c_str());
	return 0;
}

GLuint Shader::GetAttribLocation(const std::string& str)
{
	return glGetAttribLocation(m_program, str.c_str());
}

GLuint Shader::GetUniformLocation(const std::string& str)
{
	return glGetUniformLocation(m_program, str.c_str());
}

void Shader::SetShaderID(GLuint id)
{
	m_shaderId = id;
}
GLuint Shader::GetShaderID()
{
	return m_shaderId;
}
void Shader::EnableTest(GLuint gl_test)
{
	glEnable(gl_test);
}
void Shader::EnableAlpha() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}