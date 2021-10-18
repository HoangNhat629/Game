#include "Application.h"
#include "GameStates/GameStateMachine.h"
#include "GameStates/GameStatebase.h"

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create a 2D camera
	m_camera = std::make_shared<Camera>(0, 0.0f, (float)Globals::screenWidth, 0.0f, (float)Globals::screenHeight, -1.0f, 1.0f, 10.0f);

	GameStateMachine::GetInstance()->PushState(StateType::STATE_INTRO);
}

void Application::Update(GLfloat deltaTime)
{
	GameStateMachine::GetInstance()->PerformStateChange();

	if (GameStateMachine::GetInstance()->HasState())
		GameStateMachine::GetInstance()->CurrentState()->Update(deltaTime);

	m_inputManager.OnUpdate();
}

void Application::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (GameStateMachine::GetInstance()->HasState())
		GameStateMachine::GetInstance()->CurrentState()->Draw();
}

void Application::HandleKeyEvent(unsigned char key, bool bIsPressed)
{
	m_inputManager.OnKeyEvent(key, bIsPressed);

	if (GameStateMachine::GetInstance()->HasState())
		GameStateMachine::GetInstance()->CurrentState()->HandleKeyEvents(key, bIsPressed);
}

void Application::HandleTouchEvent(GLint x, GLint y, bool bIsPressed)
{
	m_inputManager.OnTouchEvent(bIsPressed);

	if (GameStateMachine::GetInstance()->HasState())
		GameStateMachine::GetInstance()->CurrentState()->HandleTouchEvents(x, y, bIsPressed);
}

void Application::HandleMouseMoveEvent(GLint x, GLint y)
{
	if (GameStateMachine::GetInstance()->HasState())
		GameStateMachine::GetInstance()->CurrentState()->HandleMouseMoveEvents(x, y);
}

void Application::Exit()
{
	exit(0);
}
