#pragma once

#include "StateBase.h"

namespace Agvt
{
    class PlayState : public StateBase
    {
    public:
        PlayState(StateMachine* managingStateMachine);

        void Update(float deltaTime) override;
        void OnEnter() override;
        void OnExit() override;
    };
}
