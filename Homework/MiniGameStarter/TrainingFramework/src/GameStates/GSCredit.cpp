#include "GSCredit.h"
#include "Camera.h"

#include "../TextConstants.h"

GSCredit::GSCredit() : GameStateBase(StateType::STATE_CREDIT),
m_background(nullptr), m_panelCredits(nullptr), m_textCredits(nullptr), m_listButton{}
{
}


GSCredit::~GSCredit()
{
}



void GSCredit::Init()
{
    auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
    auto texture = ResourceManagers::GetInstance()->GetTexture("background.tga");

    // background
    auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
    m_background = std::make_shared<Sprite2D>(model, shader, texture);
    m_background->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
    m_background->SetSize((float)Globals::screenWidth, (float)Globals::screenHeight);

    // close button
    texture = ResourceManagers::GetInstance()->GetTexture("button_close.tga");
    std::shared_ptr<GameButton> buttonClose = std::make_shared<GameButton>(model, shader, texture);
    buttonClose->Set2DPosition(Globals::screenWidth - 30, 30);
    buttonClose->SetSize(50, 50);
    buttonClose->SetOnClick([]()
    {
        GameStateMachine::GetInstance()->PopState();
    });
    m_listButton.push_back(buttonClose);

    // credit panel
    texture = ResourceManagers::GetInstance()->GetTexture("panel_credits.tga");
    m_panelCredits = std::make_shared<Sprite2D>(model, shader, texture);
    m_panelCredits->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
    m_panelCredits->SetSize(450, 250);

    // credits text
    shader = ResourceManagers::GetInstance()->GetShader("TextShader");
    std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("Puzzle-Tale-Pixel-Regular.ttf");
    m_textCredits = std::make_shared<Text>(shader, font, TextConstants::CREDITS, Vector4(0.0f, 0.0f, 0.0f, 255.0f) / 255.0f, 2.0f, 1.0f);
    m_textCredits->Set2DPosition(150.0f, (float)Globals::screenHeight / 2 - 40.0f);
}

void GSCredit::Exit()
{
}


void GSCredit::Pause()
{
}

void GSCredit::Resume()
{
}


void GSCredit::HandleEvents()
{
}

void GSCredit::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSCredit::HandleTouchEvents(int x, int y, bool bIsPressed)
{
    for (auto button : m_listButton)
    {
        if (button->HandleTouchEvents(x, y, bIsPressed))
        {
            break;
        }
    }
}

void GSCredit::HandleMouseMoveEvents(int x, int y)
{
}

void GSCredit::Update(float deltaTime)
{
    m_background->Update(deltaTime);
    for (auto it : m_listButton)
    {
        it->Update(deltaTime);
    }
}

void GSCredit::Draw()
{
    m_background->Draw();
    m_panelCredits->Draw();
    m_textCredits->Draw();

    for (auto it : m_listButton)
    {
        it->Draw();
    }
}
