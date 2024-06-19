#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "title.h"
#include "titlelogo.h"
#include "Input.h"
#include "game.h"
#include "fade.h"
#include "loading.h"
#include "audio.h"


void Title::Init()
{
	//タイトルシーンに来たらフェードする
	AddGameObject<Titlelogo>(2);
	m_Fade = AddGameObject<Fade>(2);

	m_BGM = AddGameObject<GameObject>(0)->AddCompornent<Audio>();

	m_BGM->Load("asset\\audio\\BGM\\oboroduki.wav");
	m_BGM->Play(true);
}

void Title::Update()
{
	Scene::Update();

	//キー入力でゲーム画面へ遷移
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		m_Fade->FadeOut();
		m_BGM->FadeOut(2.0f); // 2秒間かけてBGMをフェードアウト
	}

	if (m_Fade->GetFadeFinish())
	{
		Manager::SetScene<Loading>();
	}
}
