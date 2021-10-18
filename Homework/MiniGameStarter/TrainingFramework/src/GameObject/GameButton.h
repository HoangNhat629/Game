#pragma once 
#include "Sprite2D.h"

#include <functional>

class GameButton : public Sprite2D
{
public:
	GameButton() : Sprite2D(), m_clickCallback{}, m_isHolding(false) {}
	GameButton(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture);
	~GameButton() = default;
	void	SetOnClick(std::function<void()> clickCallback);
	bool	HandleTouchEvents(GLint x, GLint y, bool bIsPressed);
	bool	IsHolding() const;

private:
	bool	TestPointInBounds(GLint x, GLint y) const;

private:
	std::function<void()> m_clickCallback;
	bool	m_isHolding;
};
