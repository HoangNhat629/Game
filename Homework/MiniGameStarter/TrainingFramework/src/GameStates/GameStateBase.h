#pragma once
#include "GameStateMachine.h"
#include "GameManager/ResourceManagers.h"


class GameStateBase
{
public:
	GameStateBase() : m_stateType(StateType::STATE_INVALID){}
	GameStateBase(StateType stateType);
	virtual ~GameStateBase() = default;

	virtual void Init() = 0;
	virtual void Exit() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents() = 0;
	virtual void HandleKeyEvents(int key, bool bIsPressed) = 0;
	virtual void HandleTouchEvents(int x, int y, bool bIsPressed) = 0;
	virtual void HandleMouseMoveEvents(int x, int y) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
	
	static std::shared_ptr<GameStateBase> CreateState(StateType stt);
	StateType GetGameStateType();

protected:
	StateType m_stateType;
};

