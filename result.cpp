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

	//キー入力でタイトル画面へ遷移
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		m_Fade->FadeOut();
	}

	if (m_Fade->GetFadeFinish())
	{
		Manager::SetScene<Title>();

	}
}
