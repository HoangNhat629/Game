#include "InvalidState.h"

namespace Agvt
{
    InvalidState::InvalidState(StateMachine* managingStateMachine) : StateBase(managingStateMachine)
    {
    }

    void InvalidState::Update(float deltaTime)
    {
        // Do nothing
    }

    void InvalidState::OnEnter()
    {
    }

    void InvalidState::OnExit()
    {
    }
}