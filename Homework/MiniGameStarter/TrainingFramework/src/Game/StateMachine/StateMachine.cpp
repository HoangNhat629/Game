#include "StateMachine.h"

#include "StateBase.h"
#include "InvalidState.h"
#include "PlayState.h"
#include "GravitySelectState.h"
#include "UpdateState.h"
#include "EndState.h"

#include <array>

namespace Agvt
{
    StateMachine::StateMachine() : m_states{}, m_previousStateType(StateType::Invalid), m_currentStateType(StateType::Invalid), m_currentState{},
        m_gameWorld{}, m_inputManager{}
    {
    }

    void StateMachine::Init(GameWorld* gameWorld, const InputManager* inputManager)
    {
        m_gameWorld = gameWorld;
        m_inputManager = inputManager;

        m_states = CreateStates();

        m_currentState = m_states[StateTypeToIndex(m_currentStateType)].get();
        m_currentState->OnEnter();
    }

    void StateMachine::Update(float deltaTime)
    {
        m_currentState->Update(deltaTime);
    }

    StateType StateMachine::GetPreviosStateType() const
    {
        return m_previousStateType;
    }

    StateType StateMachine::GetCurrentStateType() const
    {
        return m_currentStateType;
    }

    StateBase* StateMachine::GetCurrentState() const
    {
        return m_currentState;
    }
    void StateMachine::ChangeState(StateType newStateType)
    {
        m_previousStateType = m_currentStateType;

        m_currentState->OnExit();

        m_currentStateType = newStateType;
        m_currentState = m_states[StateTypeToIndex(m_currentStateType)].get();
        m_currentState->OnEnter();
    }

    GameWorld* StateMachine::GetGameWorld() const
    {
        return m_gameWorld;
    }

    const InputManager* StateMachine::GetInputManager() const
    {
        return m_inputManager;
    }

    void StateMachine::SetInputManager(const InputManager* inputManager)
    {
        m_inputManager = inputManager;
    }


    std::vector<std::unique_ptr<StateBase>> StateMachine::CreateStates()
    {
        std::vector<std::unique_ptr<StateBase>> states(c_stateTypeElementCount);

        states[StateTypeToIndex(StateType::Invalid)] = std::make_unique<InvalidState>(this);
        states[StateTypeToIndex(StateType::Playing)] = std::make_unique<PlayState>(this);
        states[StateTypeToIndex(StateType::GravitySelecting)] = std::make_unique<GravitySelectState>(this);
        states[StateTypeToIndex(StateType::Updating)] = std::make_unique<UpdateState>(this);
        states[StateTypeToIndex(StateType::Ended)] = std::make_unique<EndState>(this);

        return states;
    }

    constexpr size_t StateMachine::StateTypeToIndex(StateType stateType)
    {
        return (size_t)stateType;
    }
}