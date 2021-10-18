#pragma once
#include "BaseObject.h"


class Sprite3D : public BaseObject
{
public:
	Sprite3D(GLint id, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture);
	Sprite3D(GLint id, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, Vector4 color);
	~Sprite3D();

	void		Init() override;
	void		Draw() override;
};

