#include "GravitySelectState.h"

#include "StateMachine.h"
#include "../GameWorld.h"

#include <array>

namespace Agvt
{
    constexpr float Lerp(float a, float b, float t);

    GravitySelectState::GravitySelectState(StateMachine* managingStateMachine) : StateBase(managingStateMachine),
        m_sourceAngle{}, m_targetAngle{}, m_animationTimer{}, m_inAnimation(false)
    {
    }

    void GravitySelectState::Update(float deltaTime)
    {
        if (m_inAnimation)
        {
            UpdateAnimation(deltaTime);
        }
        else
        {
            HandlePlayerInput();
        }
    }

    void GravitySelectState::OnEnter()
    {
        GameWorld* gameWorld = m_managingStateMachine->GetGameWorld();
        gameWorld->m_player->SetAnimationState(PlayerEntity::AnimationState::GravityStop);
    }

    void GravitySelectState::OnExit()
    {
    }


    void GravitySelectState::HandlePlayerInput()
    {
        const InputManager* inputManager = m_managingStateMachine->GetInputManager();
        GameWorld* gameWorld = m_managingStateMachine->GetGameWorld();

        if (inputManager->KeyDown(KeyCode::LEFT))
        {
            BeginRotationAnimation((size_t)(gameWorld->m_currentGravityDirection), (size_t)GameWorld::GravityDirection::Left);
            gameWorld->SetGravity(GameWorld::GravityDirection::Left);
        }
        else if (inputManager->KeyDown(KeyCode::RIGHT))
        {
            BeginRotationAnimation((size_t)(gameWorld->m_currentGravityDirection), (size_t)GameWorld::GravityDirection::Right);
            gameWorld->SetGravity(GameWorld::GravityDirection::Right);
        }
        else if (inputManager->KeyDown(KeyCode::SPACE))
        {
            m_managingStateMachine->ChangeState(StateType::Updating);
        }
    }

    void GravitySelectState::UpdateAnimation(float deltaTime)
    {
        GameWorld* gameWorld = m_managingStateMachine->GetGameWorld();

        constexpr float c_animationTime = 0.1f;

        m_animationTimer += deltaTime;
        if (m_animationTimer >= c_animationTime)
        {
            gameWorld->SetWorldRotation(m_targetAngle >= 0.0f ? m_targetAngle : m_targetAngle + 2.0f * PI);

            m_inAnimation = false;
            m_animationTimer = 0;
        }
        else
        {
            gameWorld->SetWorldRotation(Lerp(m_sourceAngle, m_targetAngle, m_animationTimer / c_animationTime));
        }
    }

    void GravitySelectState::BeginRotationAnimation(size_t currentGravityIndex, size_t shiftGravityIndex)
    {
        constexpr std::array<float, 4> directionToZAngle
        {
            0.0f,           // Down
            PI * 0.5f,      // Right
            PI * 1.0f,      // Up
            PI * 1.5f       // Left
        };

        constexpr std::array<float, 4> directionToZAngleOffset
        {
            0.0f,           // Down
            PI * 0.5f,      // Right
            PI * 1.0f,      // Up
            PI * (-0.5f)    // Left
        };


        float currentZAngle = directionToZAngle[currentGravityIndex];
        float offsetZAngle = directionToZAngleOffset[shiftGravityIndex];

        m_sourceAngle = currentZAngle;
        m_targetAngle = currentZAngle + offsetZAngle;

        m_inAnimation = true;
    }

    constexpr float Lerp(float a, float b, float t)
    {
        return a * (1 - t) + b * t;
    }
}