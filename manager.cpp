#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "game.h"
#include "title.h"
#include "audio.h"
#include "depthpolygon.h"
#include "meshField.h"
#include "player.h"

CPolygon* g_Polygon;
MeshField* g_Field;
Player* g_Player;
/*
Gameobject*    g_Camera{};
Gameobject*    g_Field{};
Gameobject*    g_Player{};
Gameobject*    g_Polygon{};
*/

//�ÓI�����o�ϐ��͍Đ錾���K�v
Scene* Manager::m_Scene{};
Scene* Manager::m_NextScene{};

void Manager::Init()
{
	Audio::InitMaster();

	Renderer::Init();

	Input::Init();

	

	//�ŏ��̓^�C�g����ʂ��Z�b�g
	SetScene<Title>();
}

void Manager::UnInit()
{
	m_Scene->UnInit();
	delete m_Scene;

	Audio::UninitMaster();

	Input::UnInit();

	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();

	if (m_NextScene)
	{
		if (m_Scene)
		{
			m_Scene->UnInit();
			delete m_Scene;
		}
		m_Scene = m_NextScene;
		m_NextScene->Init();

		m_NextScene = nullptr;
	}


	m_Scene->Update();
}

void Manager::Draw()
{
	//���C�g�J�����\���̂̏����� �ŏ��̓��C�g�֘A�̐��l�����̒l�ɂ��Ă���
	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���C�g�J�����̃r���[�s����쐬
	D3DXVECTOR3 lightPos = D3DXVECTOR3(-40.0f, 40.0f, -40.0f);//10/11�����̐��l�����������ɒ���
	D3DXVECTOR3 lightTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 lightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&light.ViewMatrix, &lightPos, &lightTarget, &lightUp);

	//���C�g�J�����̃v���W�F�N�V�����s����쐬
	D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 10.0f, 100.0f);
	//���C�g�����Z�b�g
	Renderer::SetLight(light);

	//**�P�p�X�� �V���h�E�o�b�t�@�̍쐬**
	Renderer::BeginDepth();
	Renderer::SetDepthViewport();

	//���C�g�J�����̍s����Z�b�g
	Renderer::SetViewMatrix(&light.ViewMatrix);
	Renderer::SetProjectionMatrix(&light.ProjectionMatrix);
	//�e�𗎂Ƃ������I�u�W�F�N�g��`��(�ꉞ�n�ʂ�)
	m_Scene->DepthDraw();
	//g_Field= m_Scene->AddGameObject<MeshField>(1);
	//g_Player = m_Scene->AddGameObject<Player>(1);

	//**2�p�X�� �ʏ�̕`��**
	Renderer::Begin();
	Renderer::SetDefaltViewport();

	m_Scene->Draw();

	light.Enable = false;
	Renderer::SetLight(light);
	//�X�v���C�g�`��i�[�x�o�b�t�@�̓��e�������j
	//g_Polygon->Draw();
	Renderer::End();
}
