#pragma once
#include "GameStateBase.h"

#include "../Game/Editor/GameWorldEditor.h"

#include "ApplicationManagers/InputManager.h"
#include "ApplicationManagers/SessionManager.h"

class Sprite2D;
class Sprite3D;
class Text;
class GameButton;

class GSLevelEditor :
    public GameStateBase
{
public:
    GSLevelEditor();
    ~GSLevelEditor() = default;

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
    void ToggleDrawingMode();

    void RestartEditor();
    void Save();

private:
    std::unique_ptr<Sprite2D> m_background;
    std::vector<std::unique_ptr<GameButton>> m_listButton;

    const InputManager* m_inputManager;
    bool m_inDrawingMode;

    int m_selectedLevelID;


    Agvt::GameWorldEditor m_gameWorldEditor;
};

