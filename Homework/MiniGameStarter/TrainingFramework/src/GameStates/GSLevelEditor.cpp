#include "GSLevelEditor.h"

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

GSLevelEditor::GSLevelEditor() : GameStateBase(StateType::STATE_LEVEL_EDITOR), m_selectedLevelID(-1), m_inputManager{}, m_inDrawingMode(true)
{
}

void GSLevelEditor::Init()
{
    m_inputManager = Application::GetInstance()->GetInputManager();

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
        RestartEditor();
    });
    m_listButton.push_back(std::move(restartButton));

    // button restart
    texture = ResourceManagers::GetInstance()->GetTexture("button_save.tga");
    std::unique_ptr<GameButton> saveButton = std::make_unique<GameButton>(model, shader, texture);
    saveButton->Set2DPosition(100, 30);
    saveButton->SetSize(50, 50);
    saveButton->SetOnClick([this]()
    {
        Save();
    });
    m_listButton.push_back(std::move(saveButton));

    m_selectedLevelID = Application::GetInstance()->GetSessionManager()->GetSelectedLevel();

    RestartEditor();
}

void GSLevelEditor::Exit()
{
}


void GSLevelEditor::Pause()
{
}

void GSLevelEditor::Resume()
{
}


void GSLevelEditor::HandleEvents()
{
}

void GSLevelEditor::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSLevelEditor::HandleTouchEvents(int x, int y, bool bIsPressed)
{
    if (!m_inDrawingMode)
    {
        for (auto& button : m_listButton)
        {
            if (button->HandleTouchEvents(x, y, bIsPressed))
            {
                return;
            }
        }
    }

    m_gameWorldEditor.HandleTouchEvents(x, y, bIsPressed);
}

void GSLevelEditor::HandleMouseMoveEvents(int x, int y)
{
    m_gameWorldEditor.HandleMouseMoveEvents(x, y);
}

void GSLevelEditor::Update(float deltaTime)
{
    if (m_inputManager->KeyDown(KeyCode::E))
    {
        ToggleDrawingMode();
    }
    
    if (m_inDrawingMode)
    {
        m_gameWorldEditor.Update(deltaTime);
    }
}

void GSLevelEditor::Draw()
{
    m_background->Draw();

    m_gameWorldEditor.Draw();

    if (!m_inDrawingMode)
    {
        for (auto& it : m_listButton)
        {
            it->Draw();
        }
    }
}

void GSLevelEditor::ToggleDrawingMode()
{
    m_inDrawingMode = !m_inDrawingMode;
}

void GSLevelEditor::RestartEditor()
{
    m_gameWorldEditor.LoadFromFile(m_selectedLevelID);
}

void GSLevelEditor::Save()
{
    m_gameWorldEditor.SaveToFile(m_selectedLevelID);
}
