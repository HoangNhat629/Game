#pragma once
#include "GameStateBase.h"
#include "GameButton.h"

#include "ApplicationManagers/SoundManager.h"

class GSMenu :
    public GameStateBase
{
public:
    GSMenu();

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
    void ToggleSoundOnOff(bool on);

private:
    std::unique_ptr<Sprite2D> m_background;
    std::vector<std::unique_ptr<GameButton>> m_listButton;
    std::unique_ptr<Text> m_textGameName;

    GameButton* m_buttonSoundOn;
    GameButton* m_buttonSoundOff;

    SoundManager* m_soundManager;
};

