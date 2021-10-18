#include "UpdateState.h"

#include "StateMachine.h"
#include "../GameWorld.h"

namespace Agvt
{
    constexpr float c_updateTime = 0.2f;
    constexpr float c_initialTimerValueAfterPlayerMovement = 0.1f;

    UpdateState::UpdateState(StateMachine* managingStateMachine) : StateBase(managingStateMachine), m_updateTimer{}
    {
    }

    void UpdateState::Update(float deltaTime)
    {
        GameWorld* gameWorld = m_managingStateMachine->GetGameWorld();

        m_updateTimer += deltaTime;
        if (m_updateTimer >= c_updateTime)
        {
            bool needToUpdate = gameWorld->ApplyGravity();

            if (needToUpdate)
            {
                m_updateTimer -= c_updateTime;
            }
            else
            {
                if (gameWorld->IsGameOver() || gameWorld->IsVictory())
                {
                    m_managingStateMachine->ChangeState(StateType::Ended);
                }
                else
                {
                    m_managingStateMachine->ChangeState(StateType::Playing);
                }
            }
        }
    }

    void UpdateState::OnEnter()
    {
        bool playerMoveLastState = m_managingStateMachine->GetPreviosStateType() == StateType::Playing;
        m_updateTimer = playerMoveLastState ? c_initialTimerValueAfterPlayerMovement : 0.0f;

        GameWorld* gameWorld = m_managingStateMachine->GetGameWorld();
        gameWorld->MarkWorldAsChanged();
    }

    void UpdateState::OnExit()
    {
        GameWorld* gameWorld = m_managingStateMachine->GetGameWorld();
        gameWorld->ProcessEntityRemovePendingList();
    }
}