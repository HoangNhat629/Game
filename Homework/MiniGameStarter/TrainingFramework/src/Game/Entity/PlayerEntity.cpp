#include "PlayerEntity.h"

#include "AtlasSprite2D.h"

constexpr float c_idleFrameTime = 0.15f;
constexpr int c_idleFrameCount = 4;
constexpr int c_fallingFrameNumber = 4;
constexpr int c_gravityStopFrameNumber = 5;

constexpr int c_facingRightAtlasRow = 1;
constexpr int c_facingLeftAtlasRow = 0;

namespace Agvt
{
    PlayerEntity::PlayerEntity(std::unique_ptr<Sprite2D> sprite) : Entity(EntityType::Player, std::move(sprite)),
        m_currentFrameTime{}, m_currentIdleFrameNumber{}, m_isFacingRight(true), m_currentAnimationState(AnimationState::Idle)
    {
        UpdateAnimation();
    }

    void PlayerEntity::SetFacingDirection(bool right)
    {
        m_isFacingRight = right;
        UpdateAnimation();
    }

    void PlayerEntity::SetAnimationState(AnimationState animationState)
    {
        m_currentAnimationState = animationState;

        UpdateAnimation();
    }

    void PlayerEntity::Update(float deltaTime)
    {
        m_currentFrameTime += deltaTime;

        if (m_currentFrameTime >= c_idleFrameTime)
        {
            m_currentIdleFrameNumber++;
            if (m_currentIdleFrameNumber >= c_idleFrameCount)
            {
                m_currentIdleFrameNumber = 0;
            }

            UpdateAnimation();

            m_currentFrameTime -= c_idleFrameTime;
        }
    }

    void PlayerEntity::UpdateAnimation()
    {
        AtlasSprite2D* atlasSprite2D = static_cast<AtlasSprite2D*>(m_sprite.get());

        switch (m_currentAnimationState)
        {
        case PlayerEntity::AnimationState::Idle:
            atlasSprite2D->SetAtlasCoord(Vector2Int(m_currentIdleFrameNumber, m_isFacingRight ? c_facingRightAtlasRow : c_facingLeftAtlasRow));
            break;
        case PlayerEntity::AnimationState::Falling:
            atlasSprite2D->SetAtlasCoord(Vector2Int(c_fallingFrameNumber, m_isFacingRight ? c_facingRightAtlasRow : c_facingLeftAtlasRow));
            break;
        case PlayerEntity::AnimationState::GravityStop:
            atlasSprite2D->SetAtlasCoord(Vector2Int(c_gravityStopFrameNumber, m_isFacingRight ? c_facingRightAtlasRow : c_facingLeftAtlasRow));
            break;
        default:
            break;
        }
    }

}