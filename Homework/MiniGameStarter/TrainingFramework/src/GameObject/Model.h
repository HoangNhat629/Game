#pragma once
#include "GameConfig.h"

struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector3 bitangent;
	Vector3 tangent;
	Vector2 uv;
};

class Model
{
public:
	Model(const std::string& fileName, GLuint type);
	~Model();
	GLuint		GetVertexObject();
	GLuint		GetIndiceObject();
	GLuint		GetNumIndiceObject();
	void		SetModelId(GLuint id);
	GLuint		GetModelId();

private:
	GLuint		m_modelId;
	GLuint		m_numIndices;
	GLuint		m_vboId;
	GLuint		m_iboId;
};