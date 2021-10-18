#include "PlayState.h"

#include "StateMachine.h"
#include "../GameWorld.h"

namespace Agvt
{
    PlayState::PlayState(StateMachine* managingStateMachine) : StateBase(managingStateMachine)
    {
    }

    void PlayState::Update(float deltaTime)
    {
        const InputManager* inputManager = m_managingStateMachine->GetInputManager();
        GameWorld* gameWorld = m_managingStateMachine->GetGameWorld();

        if (inputManager->KeyDown(KeyCode::LEFT))
        {
            if (gameWorld->TryMovePlayerUnderInput(GameWorld::InputDirection::Left))
            {
                m_managingStateMachine->ChangeState(StateType::Updating);
            } 
        }
        else if (inputManager->KeyDown(KeyCode::RIGHT))
        {
            if (gameWorld->TryMovePlayerUnderInput(GameWorld::InputDirection::Right))
            {
                m_managingStateMachine->ChangeState(StateType::Updating);
            }
        }
        else if (inputManager->KeyDown(KeyCode::SPACE))
        {
            m_managingStateMachine->ChangeState(StateType::GravitySelecting);
        }
    }
    void PlayState::OnEnter()
    {
    }
    void PlayState::OnExit()
    {
    }
}