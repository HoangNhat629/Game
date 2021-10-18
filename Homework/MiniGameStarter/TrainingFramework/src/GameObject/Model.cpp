#include "Model.h"
#include "utilities.h" 

Model::Model(const std::string& fileName, GLuint type) : m_modelId(0), m_numIndices(0), m_vboId(0), m_iboId(0)
{

	std::ifstream modelFile(fileName, std::ios::binary);
	if (!modelFile.is_open())
	{
		LOGE("ERROR %s \n", fileName.c_str());
		return;
	}
	LOGI("Load Models:%s\t\t", fileName.c_str());

	GLuint iNumVertex = 0;
	Vertex* verticesData{};
	GLuint* Indices{};
	if (type == NFG)
	{
		char* buffer;
		std::string strtmp;
		GLfloat posX, posY, posZ, normX, normY, normZ, binormX, binormY, binormZ, tgtX, tgtY, tgtZ, uvX, uvY;
		//read data from file
		modelFile.seekg(0, modelFile.end);
		size_t bufSize = (size_t)modelFile.tellg();
		modelFile.seekg(0, modelFile.beg);

		buffer = new char[bufSize + 1];
		modelFile.read(buffer, bufSize);

		buffer[bufSize] = '\0';
		std::stringstream ss(buffer);

		std::getline(ss, strtmp, '\n');
		GLint rs = sscanf(strtmp.c_str(), "NrVertices: %d", &iNumVertex);
		verticesData = new Vertex[iNumVertex];
		for (GLuint i = 0; i < iNumVertex; i++)
		{
			std::getline(ss, strtmp, '\n');
			rs = sscanf(strtmp.c_str(), "   %*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];", &posX, &posY, &posZ, &normX, &normY, &normZ, &binormX, &binormY, &binormZ, &tgtX, &tgtY, &tgtZ, &uvX, &uvY);
			verticesData[i].position.x = posX;
			verticesData[i].position.y = posY;
			verticesData[i].position.z = posZ;

			verticesData[i].normal.x = normX;
			verticesData[i].normal.y = normY;
			verticesData[i].normal.z = normZ;

			verticesData[i].bitangent.x = binormX;
			verticesData[i].bitangent.y = binormY;
			verticesData[i].bitangent.z = binormZ;

			verticesData[i].tangent.x = binormX;
			verticesData[i].tangent.y = binormY;
			verticesData[i].tangent.z = binormZ;

			verticesData[i].uv.x = uvX;
			verticesData[i].uv.y = uvY;
		}
		std::getline(ss, strtmp, '\n');

		rs = sscanf(strtmp.c_str(), "NrIndices: %d", &m_numIndices);
		Indices = new GLuint[m_numIndices];
		GLuint a, b, c;
		for (GLuint i = 0; i < m_numIndices / 3; i++)
		{
			std::getline(ss, strtmp, '\n');
			rs = sscanf(strtmp.c_str(), " %*d.  %d,  %d,  %d", &a, &b, &c);
			Indices[3 * i + 0] = a;
			Indices[3 * i + 1] = b;
			Indices[3 * i + 2] = c;
		}
	}

	//buffer object
	glGenBuffers(1, &m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferData(GL_ARRAY_BUFFER, iNumVertex * sizeof(Vertex), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &m_iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(GLuint), Indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	modelFile.close();
	LOGI("Done.\n");
}


Model::~Model()
{
}

GLuint Model::GetVertexObject()
{
	return m_vboId;
}
GLuint Model::GetIndiceObject()
{
	return m_iboId;
}
GLuint Model::GetNumIndiceObject()
{
	return m_numIndices;
}
void Model::SetModelId(GLuint id)
{
	m_modelId = id;
}
GLuint Model::GetModelId()
{
	return m_modelId;
}