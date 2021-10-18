#pragma once

#include "StateBase.h"

#include <stdint.h>

namespace Agvt
{
    class GravitySelectState : public StateBase
    {
    public:
        GravitySelectState(StateMachine* managingStateMachine);

        void Update(float deltaTime) override;
        void OnEnter() override;
        void OnExit() override;

    private:
        void HandlePlayerInput();
        void UpdateAnimation(float deltaTime);

        void BeginRotationAnimation(size_t currentGravityIndex, size_t shiftGravityIndex);

    private:
        float m_sourceAngle;
        float m_targetAngle;

        float m_animationTimer;
        bool m_inAnimation;
    };
}
