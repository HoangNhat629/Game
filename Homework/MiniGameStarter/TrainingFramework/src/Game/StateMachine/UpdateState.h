#pragma once

#include "StateBase.h"

namespace Agvt
{
    class UpdateState : public StateBase
    {
    public:
        UpdateState(StateMachine* managingStateMachine);

        void Update(float deltaTime) override;
        void OnEnter() override;
        void OnExit() override;

    private:
        float m_updateTimer;
    };
}
