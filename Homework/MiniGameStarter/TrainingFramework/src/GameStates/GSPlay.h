#pragma once
#include "GameStateBase.h"

#include "../Game/GameWorld.h"

#include "ApplicationManagers/InputManager.h"
#include "ApplicationManagers/SessionManager.h"

class Sprite2D;
class Sprite3D;
class Text;
class GameButton;

class GSPlay : public GameStateBase
{
private:
    enum class State
    {
        Play,
        Victory
    };

public:
    GSPlay();
    ~GSPlay() = default;

    void Init() override;
    void Exit() override;

    void Pause() override;
    void Resume() override;

    void HandleEvents() override;
    void HandleKeyEvents(int key, bool bIsPressed) override;
    void HandleTouchEvents(int x, int y, bool bIsPressed) override;
    void HandleMouseMoveEvents(int x, int y) override;
    void Update(float deltaTime) override;
    void Draw() override;

private:
    void RestartGameWorld();
    void OnVictory();

    void OnPlayStateUpdate(float deltaTime);
    void OnVictoryStateUpdate(float deltaTime);

private:
    std::unique_ptr<Sprite2D> m_background;
    std::vector<std::unique_ptr<GameButton>> m_listButton;

    std::unique_ptr<Sprite2D> m_victoryPanel;

    Agvt::GameWorld m_gameWorld;
    const InputManager* m_inputManager;
    const SessionManager* m_sessionManager;

    State m_state;
    float m_victoryWaitTime;
};

