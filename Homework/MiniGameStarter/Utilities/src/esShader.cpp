#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "targetver.h"

#ifndef ANDROID
#include "esUtil.h"
#endif //android
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#ifdef ANDROID
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif //android

/// \brief Load a shader, check for compile errors, print error messages to output log
/// \param type Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
/// \param shaderSrc Shader source string
/// \return A new shader object on success, 0 on failure
//
#ifndef ANDROID
GLuint ESUTIL_API esLoadShader(GLenum type, const std::string& filename)
#else
GLuint esLoadShader(GLenum type, const std::string& filename)
#endif //android
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader(type);

	if (shader == 0)
		return 0;

	// Load the shader source
#ifndef ANDROID
	FILE* pf;
	if (fopen_s(&pf, filename.c_str(), "rb") != 0)
		return NULL;

#else
	FILE* pf = fopen(filename, "rb");
	if (pf == NULL)
	{
		return 0;
	}
#endif //android
	fseek(pf, 0, SEEK_END);
	long size = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	char* shaderSrc = new char[size + 1];
	fread(shaderSrc, sizeof(char), size, pf);
	shaderSrc[size] = 0;
	fclose(pf);

	glShaderSource(shader, 1, (const char**)&shaderSrc, NULL);
	delete[] shaderSrc;

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[infoLen];


			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
#ifndef ANDROID
			esLogMessage("Error compiling shader:\n%s\n", infoLog);
#else
			__android_log_print(ANDROID_LOG_ERROR, "GAME", "Error linking program:\n%s\n", infoLog);
#endif //android
			delete[] infoLog;
		}

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

/// \brief Create a program object, link program.
//         Errors output to log.
/// \param vertShaderSrc Vertex shader source code
/// \param fragShaderSrc Fragment shader source code
/// \return A new program object linked with the vertex/fragment shader pair, 0 on failure

#ifndef ANDROID
GLuint ESUTIL_API esLoadProgram(GLuint vertexShader, GLuint fragmentShader)
#else
GLuint esLoadProgram(GLuint vertexShader, GLuint fragmentShader)
#endif //android
{
	GLuint programObject;
	GLint linked;

	// Create the program object
	programObject = glCreateProgram();

	if (programObject == 0)
		return 0;

	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);

	// Link the program
	glLinkProgram(programObject);

	// Check the link status
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLint infoLen = 0;

		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[sizeof(char) * infoLen];


			glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
#ifndef ANDROID
			esLogMessage("Error linking program:\n%s\n", infoLog);
#else
			__android_log_print(ANDROID_LOG_ERROR, "CUONG.NV", "Error linking program:\n%s\n", infoLog);
#endif //android
			delete[] infoLog;
		}

		glDeleteProgram(programObject);
		return 0;
	}

	return programObject;
}
