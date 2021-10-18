#pragma once

namespace Agvt
{
    class StateMachine;

    class StateBase
    {
    public:
        StateBase(StateMachine* managingStateMachine);
        virtual ~StateBase() = default;

        virtual void Update(float deltaTime) = 0;

        virtual void OnEnter() = 0;
        virtual void OnExit() = 0;

    protected:
        StateMachine* m_managingStateMachine;
    };
}
