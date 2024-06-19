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
	//�^�C�g���V�[���ɗ�����t�F�[�h����
	AddGameObject<Titlelogo>(2);
	m_Fade = AddGameObject<Fade>(2);

	m_BGM = AddGameObject<GameObject>(0)->AddCompornent<Audio>();

	m_BGM->Load("asset\\audio\\BGM\\oboroduki.wav");
	m_BGM->Play(true);
}

void Title::Update()
{
	Scene::Update();

	//�L�[���͂ŃQ�[����ʂ֑J��
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		m_Fade->FadeOut();
		m_BGM->FadeOut(2.0f); // 2�b�Ԃ�����BGM���t�F�[�h�A�E�g
	}

	if (m_Fade->GetFadeFinish())
	{
		Manager::SetScene<Loading>();
	}
}
