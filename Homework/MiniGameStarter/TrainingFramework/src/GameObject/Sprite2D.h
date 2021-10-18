#pragma once
#include "BaseObject.h"

class Sprite2D : public BaseObject
{
protected:
	GLfloat		m_width;
	GLfloat		m_height;
	GLuint		m_vboId;

public:
	Sprite2D() : BaseObject(), m_width{}, m_height{}, m_vboId(0) {}
	Sprite2D(GLint id, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture);
	Sprite2D(GLint id, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, Vector4 color);
	Sprite2D(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture);
	~Sprite2D();

	void		Init() override;
	void		Draw() override;

	void		Set2DPosition(GLint x, GLint y);
	void		Set2DPosition(Vector2 position);
	void		SetSize(GLfloat width, GLfloat height);

	virtual std::unique_ptr<Sprite2D> Clone() const;
};

