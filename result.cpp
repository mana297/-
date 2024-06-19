#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "result.h"
#include "resultlogo.h"
#include "title.h"
#include "titlelogo.h"
#include "result.h"
#include "resultlogo.h"
#include "Input.h"
#include "game.h"
#include  "fade.h"

void Result::Init()
{
	AddGameObject<Resultlogo>(2);
	m_Fade = AddGameObject<Fade>(2);
}

void Result::Update()
{
	Scene::Update();

	//�L�[���͂Ń^�C�g����ʂ֑J��
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		m_Fade->FadeOut();
	}

	if (m_Fade->GetFadeFinish())
	{
		Manager::SetScene<Title>();

	}
}
