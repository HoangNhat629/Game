#include "EndState.h"

#include "StateMachine.h"
#include "../GameWorld.h"

namespace Agvt
{
    EndState::EndState(StateMachine* managingStateMachine) : StateBase(managingStateMachine)
    {
    }

    void EndState::Update(float deltaTime)
    {
        // Do nothing
    }
    void EndState::OnEnter()
    {
    }
    void EndState::OnExit()
    {
    }
}