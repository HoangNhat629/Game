#pragma once

#include "GameStateBase.h"

#include "ApplicationManagers/InputManager.h"

#include <vector>

class Sprite2D;
class Text;
class GameButton;

class GSLevelSelect : public GameStateBase
{
public:
    GSLevelSelect();

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
    void PlaceLevelButtons();

    void OnLevelSelected(int levelID);

private:
    std::unique_ptr<Sprite2D> m_background;
    std::unique_ptr<Sprite2D> m_titleLevel;

    std::vector<std::unique_ptr<GameButton>> m_listButton;
    std::vector<std::unique_ptr<Text>> m_listText;

    const InputManager* m_inputManager;

    bool m_inEditorMode;
};