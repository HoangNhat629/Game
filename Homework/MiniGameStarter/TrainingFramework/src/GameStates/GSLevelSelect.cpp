#include "GSLevelSelect.h"

#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "Camera.h"
#include "Font.h"
#include "Sprite2D.h"
#include "Text.h"
#include "GameButton.h"
#include "Application.h"

GSLevelSelect::GSLevelSelect() : GameStateBase(StateType::STATE_LEVEL_SELECT), m_inEditorMode(false), m_inputManager{}
{
}

void GSLevelSelect::Init()
{
    m_inputManager = Application::GetInstance()->GetInputManager();

    auto resourceManager = ResourceManagers::GetInstance();

    auto model = resourceManager->GetModel("Sprite2D.nfg");
    auto shader = resourceManager->GetShader("TextureShader");

    // background
    auto texture = resourceManager->GetTexture("game_background.tga");
    m_background = std::make_unique<Sprite2D>(model, shader, texture);
    m_background->Set2DPosition(Vector2((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2));
    m_background->SetSize((float)Globals::screenWidth, (float)Globals::screenHeight);

    // button close
    texture = resourceManager->GetTexture("button_close.tga");
    std::unique_ptr<GameButton> closeButton = std::make_unique<GameButton>(model, shader, texture);
    closeButton->Set2DPosition(Globals::screenWidth - 30, 30);
    closeButton->SetSize(50, 50);
    closeButton->SetOnClick([]()
    {
        GameStateMachine::GetInstance()->PopState();
    });
    m_listButton.push_back(std::move(closeButton));

    auto titleLevelTexture = resourceManager->GetTexture("title_levels.tga");
    m_titleLevel = std::make_unique<Sprite2D>(model, shader, titleLevelTexture);
    m_titleLevel->Set2DPosition(Vector2((float)Globals::screenWidth / 2, 70.0f));
    m_titleLevel->SetSize(280.0f, 70.0f);

    PlaceLevelButtons();
}

void GSLevelSelect::Exit()
{
}

void GSLevelSelect::Pause()
{
}

void GSLevelSelect::Resume()
{
}

void GSLevelSelect::HandleEvents()
{
}

void GSLevelSelect::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSLevelSelect::HandleTouchEvents(int x, int y, bool bIsPressed)
{
    for (auto& button : m_listButton)
    {
        if (button->HandleTouchEvents(x, y, bIsPressed))
        {
            break;
        }
    }
}

void GSLevelSelect::HandleMouseMoveEvents(int x, int y)
{
}

void GSLevelSelect::Update(float deltaTime)
{
    m_inEditorMode = m_inputManager->Key(KeyCode::E);
}

void GSLevelSelect::Draw()
{
    m_background->Draw();
    m_titleLevel->Draw();

    for (auto& it : m_listButton)
    {
        it->Draw();
    }

    for (auto& it : m_listText)
    {
        it->Draw();
    }
}

void GSLevelSelect::PlaceLevelButtons()
{
    auto resourceManager = ResourceManagers::GetInstance();

    auto model = resourceManager->GetModel("Sprite2D.nfg");
    auto textureShader = resourceManager->GetShader("TextureShader");
    auto textShader = resourceManager->GetShader("TextShader");

    auto buttonTexture = resourceManager->GetTexture("button_level_number_1.tga");
    auto textFont = resourceManager->GetFont("VCR_OSD_MONO_1.001.ttf");

    constexpr float buttonSize = 80.0f;

    constexpr int buttonPerRow = 3;
    constexpr float horizontalSpacing = 130.0f;
    constexpr float verticalSpacing = 100.0f;

    constexpr float rowWidth = buttonSize * buttonPerRow + (horizontalSpacing - buttonSize) * (buttonPerRow - 1);

    int buttonCount = 12;

    const Vector2 firstButtonPosition = Vector2((float)Globals::screenWidth * 0.5f - rowWidth * 0.5f + buttonSize * 0.5f, 200.0f);
    Vector2 currentButtonPosition = firstButtonPosition;
    int buttonPlacedInRow = 0;

    for (int i = 1; i <= buttonCount; i++)
    {
        std::unique_ptr<GameButton> buttonLevel = std::make_unique<GameButton>(model, textureShader, buttonTexture);
        buttonLevel->Set2DPosition(currentButtonPosition);
        buttonLevel->SetSize(80.0f, 80.0f);
        buttonLevel->SetOnClick([i, this]()
        {
            OnLevelSelected(i);
        });

        m_listButton.push_back(std::move(buttonLevel));

        std::unique_ptr<Text> buttonText = std::make_unique<Text>(textShader, textFont, std::to_string(i), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1.7f, 1.0f, TextAlign::CENTER);
        buttonText->Set2DPosition(currentButtonPosition + Vector2(0.0f, 15.0f));

        m_listText.push_back(std::move(buttonText));

        buttonPlacedInRow++;
        if (buttonPlacedInRow >= buttonPerRow)
        {
            currentButtonPosition.x = firstButtonPosition.x;
            currentButtonPosition.y += verticalSpacing;

            buttonPlacedInRow = 0;
        }
        else
        {
            currentButtonPosition.x += horizontalSpacing;
        }
    }
}

void GSLevelSelect::OnLevelSelected(int levelID)
{
    Application::GetInstance()->GetSessionManager()->SetSelectedLevel(levelID);

    if (m_inEditorMode)
    {
        GameStateMachine::GetInstance()->ChangeState(StateType::STATE_LEVEL_EDITOR);
    }
    else
    {
        GameStateMachine::GetInstance()->ChangeState(StateType::STATE_PLAY);
    }
}
