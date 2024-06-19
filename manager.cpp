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

//静的メンバ変数は再宣言が必要
Scene* Manager::m_Scene{};
Scene* Manager::m_NextScene{};

void Manager::Init()
{
	Audio::InitMaster();

	Renderer::Init();

	Input::Init();

	

	//最初はタイトル画面をセット
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
	//ライトカメラ構造体の初期化 最初はライト関連の数値をこの値にしておく
	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトカメラのビュー行列を作成
	D3DXVECTOR3 lightPos = D3DXVECTOR3(-40.0f, 40.0f, -40.0f);//10/11ここの数値をいい感じに調整
	D3DXVECTOR3 lightTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 lightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&light.ViewMatrix, &lightPos, &lightTarget, &lightUp);

	//ライトカメラのプロジェクション行列を作成
	D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 10.0f, 100.0f);
	//ライト情報をセット
	Renderer::SetLight(light);

	//**１パス目 シャドウバッファの作成**
	Renderer::BeginDepth();
	Renderer::SetDepthViewport();

	//ライトカメラの行列をセット
	Renderer::SetViewMatrix(&light.ViewMatrix);
	Renderer::SetProjectionMatrix(&light.ProjectionMatrix);
	//影を落としたいオブジェクトを描画(一応地面も)
	m_Scene->DepthDraw();
	//g_Field= m_Scene->AddGameObject<MeshField>(1);
	//g_Player = m_Scene->AddGameObject<Player>(1);

	//**2パス目 通常の描画**
	Renderer::Begin();
	Renderer::SetDefaltViewport();

	m_Scene->Draw();

	light.Enable = false;
	Renderer::SetLight(light);
	//スプライト描画（深度バッファの内容を可視化）
	//g_Polygon->Draw();
	Renderer::End();
}
