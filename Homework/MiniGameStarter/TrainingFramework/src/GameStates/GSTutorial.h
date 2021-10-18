#pragma once
#include "GameStateBase.h"
#include "GameButton.h"

class GSTutorial :
    public GameStateBase
{
public:
    GSTutorial();
    ~GSTutorial() = default;

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
    void LoadPanels();

    void SwitchPanel(bool forward);
    void CheckFirstLastPanel();

private:
    std::vector<std::unique_ptr<Sprite2D>> m_panels;
    std::vector<std::unique_ptr<GameButton>> m_listButton;

    GameButton* m_buttonLeft;
    GameButton* m_buttonRight;

    size_t m_currentPanelIndex;
};

