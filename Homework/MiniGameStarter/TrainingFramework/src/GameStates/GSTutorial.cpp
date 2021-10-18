#include "GSTutorial.h"
#include "Camera.h"

#include <array>

GSTutorial::GSTutorial() : GameStateBase(StateType::STATE_TUTORIAL), m_listButton{}, m_buttonLeft{}, m_buttonRight{},
m_currentPanelIndex{}
{
}

void GSTutorial::Init()
{
    auto resourceManager = ResourceManagers::GetInstance();
    auto model = resourceManager->GetModel("Sprite2D.nfg");
    auto shader = resourceManager->GetShader("TextureShader");

    // close button
    auto texture = resourceManager->GetTexture("button_close.tga");
    std::unique_ptr<GameButton> buttonClose = std::make_unique<GameButton>(model, shader, texture);
    buttonClose->Set2DPosition(Globals::screenWidth - 30, 30);
    buttonClose->SetSize(50, 50);
    buttonClose->SetOnClick([]()
    {
        GameStateMachine::GetInstance()->PopState();
    });
    m_listButton.push_back(std::move(buttonClose));

    texture = resourceManager->GetTexture("button_left.tga");
    std::unique_ptr<GameButton> buttonLeft = std::make_unique<GameButton>(model, shader, texture);
    buttonLeft->Set2DPosition(100, 110);
    buttonLeft->SetSize(70, 50);
    buttonLeft->SetOnClick([this]()
    {
        SwitchPanel(false);
    });
    m_buttonLeft = buttonLeft.get();
    m_listButton.push_back(std::move(buttonLeft));
    
    texture = resourceManager->GetTexture("button_right.tga");
    std::unique_ptr<GameButton> buttonRight = std::make_unique<GameButton>(model, shader, texture);
    buttonRight->Set2DPosition(Globals::screenWidth - 100, 110);
    buttonRight->SetSize(70, 50);
    buttonRight->SetOnClick([this]()
    {
        SwitchPanel(true);
    });
    m_buttonRight = buttonRight.get();
    m_listButton.push_back(std::move(buttonRight));

    LoadPanels();

    CheckFirstLastPanel();
}

void GSTutorial::Exit()
{
}


void GSTutorial::Pause()
{
}

void GSTutorial::Resume()
{
}


void GSTutorial::HandleEvents()
{
}

void GSTutorial::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSTutorial::HandleTouchEvents(int x, int y, bool bIsPressed)
{
    for (auto& button : m_listButton)
    {
        if (button->HandleTouchEvents(x, y, bIsPressed))
        {
            break;
        }
    }
}

void GSTutorial::HandleMouseMoveEvents(int x, int y)
{
}

void GSTutorial::Update(float deltaTime)
{
}

void GSTutorial::Draw()
{
    m_panels[m_currentPanelIndex]->Draw();

    for (auto& it : m_listButton)
    {
        it->Draw();
    }
}

void GSTutorial::LoadPanels()
{
    auto resourceManager = ResourceManagers::GetInstance();
    auto model = resourceManager->GetModel("Sprite2D.nfg");
    auto shader = resourceManager->GetShader("TextureShader");

    constexpr std::array<const char*, 2> panelTextureFileList = { "tutorial1.tga", "tutorial2.tga" };

    for (auto panelTextureFile : panelTextureFileList)
    {
        auto texture = resourceManager->GetTexture(panelTextureFile);
        std::unique_ptr<Sprite2D> panelSprite = std::make_unique<Sprite2D>(model, shader, texture);
        panelSprite->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
        panelSprite->SetSize((float)Globals::screenWidth, (float)Globals::screenHeight);

        m_panels.push_back(std::move(panelSprite));
    }
}

void GSTutorial::SwitchPanel(bool forward)
{
    if (forward)
    {
        if (m_currentPanelIndex == m_panels.size() - 1)
        {
            m_currentPanelIndex = 0;
        }
        else
        {
            m_currentPanelIndex++;
        }
    }
    else
    {
        if (m_currentPanelIndex == 0)
        {
            m_currentPanelIndex = m_panels.size() - 1;
        }
        else
        {
            m_currentPanelIndex--;
        }
    }

    CheckFirstLastPanel();
}

void GSTutorial::CheckFirstLastPanel()
{
    bool isFirstPanel = m_currentPanelIndex == 0;
    bool isLastPanel = m_currentPanelIndex == m_panels.size() - 1;

    m_buttonLeft->SetActive(!isFirstPanel);
    m_buttonRight->SetActive(!isLastPanel);
}
