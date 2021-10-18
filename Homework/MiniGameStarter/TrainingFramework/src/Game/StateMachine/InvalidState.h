#pragma once

#include "StateBase.h"

namespace Agvt
{
    class InvalidState : public StateBase
    {
    public:
        InvalidState(StateMachine* managingStateMachine);

        void Update(float deltaTime) override;

        void OnEnter() override;
        void OnExit() override;
    };
}
