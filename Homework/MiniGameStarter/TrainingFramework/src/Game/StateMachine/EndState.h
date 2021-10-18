#pragma once

#include "StateBase.h"

namespace Agvt
{
    class EndState : public StateBase
    {
    public:
        EndState(StateMachine* managingStateMachine);

        void Update(float deltaTime) override;
        void OnEnter() override;
        void OnExit() override;
    };
}
