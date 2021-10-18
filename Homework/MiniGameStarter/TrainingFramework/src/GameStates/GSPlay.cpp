#include "GSPlay.h"

#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "Camera.h"
#include "Font.h"
#include "Sprite2D.h"
#include "Sprite3D.h"
#include "Text.h"
#include "GameButton.h"
#include "Application.h"

#include "../Game/Map/GameMap.h"

constexpr float c_victoryWaitTime = 2.0f;

GSPlay::GSPlay() : GameStateBase(StateType::STATE_PLAY), m_state{}, m_victoryWaitTime{}
{
    m_inputManager = Application::GetInstance()->GetInputManager();
    m_sessionManager = Application::GetInstance()->GetSessionManager();
}

void GSPlay::Init()
{
    auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
    auto texture = ResourceManagers::GetInstance()->GetTexture("game_background.tga");

    // background
    auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
    m_background = std::make_unique<Sprite2D>(model, shader, texture);
    m_background->Set2DPosition(Vector2((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2));
    m_background->SetSize((float)Globals::screenWidth, (float)Globals::screenHeight);

    // button close
    texture = ResourceManagers::GetInstance()->GetTexture("button_close.tga");
    std::unique_ptr<GameButton> closeButton = std::make_unique<GameButton>(model, shader, texture);
    closeButton->Set2DPosition(Globals::screenWidth - 30, 30);
    closeButton->SetSize(50, 50);
    closeButton->SetOnClick([]()
    {
        GameStateMachine::GetInstance()->PopState();
    });
    m_listButton.push_back(std::move(closeButton));

    // button restart
    texture = ResourceManagers::GetInstance()->GetTexture("button_restart.tga");
    std::unique_ptr<GameButton> restartButton = std::make_unique<GameButton>(model, shader, texture);
    restartButton->Set2DPosition(30, 30);
    restartButton->SetSize(50, 50);
    restartButton->SetOnClick([this]()
    {
        RestartGameWorld();
    });
    m_listButton.push_back(std::move(restartButton));

    RestartGameWorld();
}

void GSPlay::Exit()
{
}


void GSPlay::Pause()
{
}

void GSPlay::Resume()
{
}


void GSPlay::HandleEvents()
{
}

void GSPlay::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSPlay::HandleTouchEvents(int x, int y, bool bIsPressed)
{
    for (auto& button : m_listButton)
    {
        if (button->HandleTouchEvents(x, y, bIsPressed))
        {
            break;
        }
    }
}

void GSPlay::HandleMouseMoveEvents(int x, int y)
{
}

void GSPlay::Update(float deltaTime)
{
    for (auto& it : m_listButton)
    {
        it->Update(deltaTime);
    }

    switch (m_state)
    {
    case GSPlay::State::Play:
        OnPlayStateUpdate(deltaTime);
        break;
    case GSPlay::State::Victory:
        OnVictoryStateUpdate(deltaTime);
        break;
    default:
        break;
    }

    if (m_inputManager->KeyDown(KeyCode::R))
    {
        RestartGameWorld();
    }
}

void GSPlay::Draw()
{
    m_background->Draw();

    m_gameWorld.Draw();

    for (auto& it : m_listButton)
    {
        it->Draw();
    }

    if (m_state == State::Victory)
    {
        m_victoryPanel->Draw();
    }
}

void GSPlay::RestartGameWorld()
{
    if (m_state != State::Play)
    {
        return;
    }

    m_gameWorld = Agvt::GameWorld();
    m_gameWorld.Init(m_sessionManager->GetSelectedLevel());
}

void GSPlay::OnVictory()
{
    m_state = State::Victory;

    auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
    auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");

    // victory panel
    auto texture = ResourceManagers::GetInstance()->GetTexture("panel_victory.tga");
    m_victoryPanel = std::make_unique<Sprite2D>(model, shader, texture);
    m_victoryPanel->Set2DPosition(Vector2((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2));
    m_victoryPanel->SetSize(450.0f, 180.0f);

    // play complete sound
    Application::GetInstance()->GetSoundManager()->PlayCompleteSound();
}

void GSPlay::OnPlayStateUpdate(float deltaTime)
{
    m_gameWorld.Update(deltaTime);

    if (m_gameWorld.IsVictory())
    {
        OnVictory();
    }
}

void GSPlay::OnVictoryStateUpdate(float deltaTime)
{
    m_victoryWaitTime += deltaTime;
    if (m_victoryWaitTime >= c_victoryWaitTime)
    {
        GameStateMachine::GetInstance()->PopState();
    }

    m_gameWorld.Update(deltaTime);
}
