#pragma once

#include "Sprite2D.h"
#include "Common/Vector2Int.h"

class AtlasSprite2D : public Sprite2D
{
public:
    AtlasSprite2D(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture, Vector2Int atlasSize);

    void Init() override;
    void Draw() override;

    void SetAtlasCoord(Vector2Int coord);

    std::unique_ptr<Sprite2D> Clone() const override;

private:
    Vector2Int m_atlasSize;
    Vector2Int m_currentAtlasCoord;
};