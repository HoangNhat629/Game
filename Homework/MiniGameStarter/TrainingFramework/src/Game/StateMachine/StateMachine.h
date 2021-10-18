#pragma once

#include "StateType.h"
#include "StateBase.h"

#include "ApplicationManagers/InputManager.h"

#include <vector>
#include <memory>
#include <stdint.h>

namespace Agvt
{
    class GameWorld;

    class StateMachine
    {
    public:
        StateMachine();

        void Init(GameWorld* gameWorld, const InputManager* inputManager);

        void Update(float deltaTime);

        StateType GetPreviosStateType() const;
        StateType GetCurrentStateType() const;

        StateBase* GetCurrentState() const;
        void ChangeState(StateType newStateType);

        GameWorld* GetGameWorld() const;

        const InputManager* GetInputManager() const;
        void SetInputManager(const InputManager* inputManager);

    private:
        std::vector<std::unique_ptr<StateBase>> CreateStates();
        static constexpr size_t StateTypeToIndex(StateType stateType);

    private:
        std::vector<std::unique_ptr<StateBase>> m_states;

        StateType m_previousStateType;
        StateType m_currentStateType;
        StateBase* m_currentState;

        GameWorld* m_gameWorld;

        const InputManager* m_inputManager;
    };
}
