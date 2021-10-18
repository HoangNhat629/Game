#include "GSMenu.h"
#include "Camera.h"

#include "Application.h"

GSMenu::GSMenu() : GameStateBase(StateType::STATE_MENU),
m_background(nullptr), m_listButton{}, m_textGameName(nullptr), m_buttonSoundOn{}, m_buttonSoundOff{}, m_soundManager{}
{
}


void GSMenu::Init()
{
    m_soundManager = Application::GetInstance()->GetSoundManager();

    auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
    auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");

    // background
    auto texture = ResourceManagers::GetInstance()->GetTexture("background.tga");
    m_background = std::make_unique<Sprite2D>(model, shader, texture);
    m_background->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
    m_background->SetSize((float)Globals::screenWidth, (float)Globals::screenHeight);

    // start button
    texture = ResourceManagers::GetInstance()->GetTexture("button_start.tga");
    std::unique_ptr<GameButton> buttonStart = std::make_unique<GameButton>(model, shader, texture);
    buttonStart->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2 - 30);
    buttonStart->SetSize(200, 70);
    buttonStart->SetOnClick([]()
    {
        GameStateMachine::GetInstance()->ChangeState(StateType::STATE_LEVEL_SELECT);
    });
    m_listButton.push_back(std::move(buttonStart));

    // tutorial button
    texture = ResourceManagers::GetInstance()->GetTexture("button_tutorial.tga");
    std::unique_ptr<GameButton> buttonTutorial = std::make_unique<GameButton>(model, shader, texture);
    buttonTutorial->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2 + 50);
    buttonTutorial->SetSize(150, 60);
    buttonTutorial->SetOnClick([]()
    {
        GameStateMachine::GetInstance()->ChangeState(StateType::STATE_TUTORIAL);
    });
    m_listButton.push_back(std::move(buttonTutorial));

    // credits button
    texture = ResourceManagers::GetInstance()->GetTexture("button_credits.tga");
    std::unique_ptr<GameButton> buttonCredits = std::make_unique<GameButton>(model, shader, texture);
    buttonCredits->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2 + 120);
    buttonCredits->SetSize(150, 60);
    buttonCredits->SetOnClick([]()
    {
        GameStateMachine::GetInstance()->ChangeState(StateType::STATE_CREDIT);
    });
    m_listButton.push_back(std::move(buttonCredits));

    // exit button
    texture = ResourceManagers::GetInstance()->GetTexture("button_close.tga");
    std::unique_ptr<GameButton> buttonExit = std::make_unique<GameButton>(model, shader, texture);
    buttonExit->Set2DPosition(Globals::screenWidth - 30, 30);
    buttonExit->SetSize(50, 50);
    buttonExit->SetOnClick([]()
    {
        exit(0);
    });
    m_listButton.push_back(std::move(buttonExit));

    // sound on button
    texture = ResourceManagers::GetInstance()->GetTexture("button_sound_on.tga");
    std::unique_ptr<GameButton> buttonSoundOn = std::make_unique<GameButton>(model, shader, texture);
    buttonSoundOn->Set2DPosition(Globals::screenWidth - 90, 30);
    buttonSoundOn->SetSize(50, 50);
    buttonSoundOn->SetOnClick([this]()
    {
        ToggleSoundOnOff(false);
    });
    m_buttonSoundOn = buttonSoundOn.get();
    m_listButton.push_back(std::move(buttonSoundOn));

    // sound off button
    texture = ResourceManagers::GetInstance()->GetTexture("button_sound_off.tga");
    std::unique_ptr<GameButton> buttonSoundOff = std::make_unique<GameButton>(model, shader, texture);
    buttonSoundOff->Set2DPosition(Globals::screenWidth - 90, 30);
    buttonSoundOff->SetSize(50, 50);
    buttonSoundOff->SetOnClick([this]()
    {
        ToggleSoundOnOff(true);
    });
    m_buttonSoundOff = buttonSoundOff.get();
    m_listButton.push_back(std::move(buttonSoundOff));


    // game title
    shader = ResourceManagers::GetInstance()->GetShader("TextShader");
    std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("Gamer.ttf");
    m_textGameName = std::make_unique<Text>(shader, font, "an-gravi-ti", Vector4(255.0f, 93.0f, 33.0f, 255.0f) / 255.0f, 3.0f);
    m_textGameName->Set2DPosition((float)Globals::screenWidth / 2 - 130, (float)Globals::screenHeight / 2 - 100);

    // toggle sound on by default
    ToggleSoundOnOff(true);

    // play background sound
    Application::GetInstance()->GetSoundManager()->PlayBackgroundSound();
}

void GSMenu::Exit()
{
}


void GSMenu::Pause()
{
}

void GSMenu::Resume()
{
}


void GSMenu::HandleEvents()
{
}

void GSMenu::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSMenu::HandleTouchEvents(int x, int y, bool bIsPressed)
{
    for (auto& button : m_listButton)
    {
        if (button->HandleTouchEvents(x, y, bIsPressed))
        {
            break;
        }
    }
}

void GSMenu::HandleMouseMoveEvents(int x, int y)
{
}

void GSMenu::Update(float deltaTime)
{
    m_background->Update(deltaTime);
    for (auto& it : m_listButton)
    {
        it->Update(deltaTime);
    }
}

void GSMenu::Draw()
{
    m_background->Draw();
    for (auto& it : m_listButton)
    {
        it->Draw();
    }
    m_textGameName->Draw();
}

void GSMenu::ToggleSoundOnOff(bool on)
{
    if (on)
    {
        m_buttonSoundOn->SetActive(true);
        m_buttonSoundOff->SetActive(false);
    }
    else
    {
        m_buttonSoundOff->SetActive(true);
        m_buttonSoundOn->SetActive(false);
    }

    m_soundManager->ToggleSoundOnOff(on);
}
