#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "scene.h"
#include "texUI.h"
#include "player.h"
#include "input.h"
#include "sprite.h"
#include "menuUI.h"
#include "meshField.h"
#include "audio.h"

void MenuUI::Init()
{
	// 初期化処理
	m_SelectedObject = MenuObject::Item;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	                                               //終端
	AddCompornent<Sprite>()->Init(100.0f , 100.0f, 1080.0f, 50.0f, "asset\\texture\\back.png");
	AddCompornent<Sprite>()->Init(100.0f, 100.0f, 1080.0f, 50.0f, "asset\\texture\\flame.png");
	
	//まだこれに設定しておく
	Sprite* backTex = AddCompornent<Sprite>();
	backTex->Init(100.0f, 200.0f, 400.0f, 300.0f, "asset\\texture\\back.png");
	backTex->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	AddCompornent<Sprite>()->Init(100.0f, 200.0f, 400.0f, 300.0f, "asset\\texture\\comingsoon.png");

	AddCompornent<TexUI>()->Init();


	//m_DropItemSE = AddCompornent<Audio>();
	//m_DropItemSE->Load("asset\\audio\\SE\\itemdrop.wav");

	//m_StandItemSE = AddCompornent<Audio>();
	//m_StandItemSE->Load("asset\\audio\\SE\\itemdrop.wav");
}

void MenuUI::UnInit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	//基底クラスのメソッド呼び出し
	GameObject::UnInit();
}

void MenuUI::Update()
{
	//基底クラスのメソッド呼び出し
	GameObject::Update();

	//// Playerクラスからフラグの状態を取得
	Player* player = Manager::GetScene()->GetGameObject<Player>();
	bool menuflag = player->GetMenuUIFlag();


	// バッグフラグがTrueの場合はテーブルオブジェクトを生成する
	if (menuflag)
	{

		// 選択中のオブジェクトを更新
		UpdateSelectedObject();

		// 選択中のオブジェクトに応じて処理を行う
		switch (m_SelectedObject) {
		case MenuObject::Item:
		{
			
		}
		break;
		case MenuObject::Quest:
		{
		
		}
		break;
		case MenuObject::Information:
		{
			
		}
		break;
		case MenuObject::System:
		{

		}
		break;
		default:
			// 何も選択されていない場合の処理
			break;
		}
	}
}


void MenuUI::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリックス設定
	Renderer::SetWorldViewProjection2D();


	//基底クラスのメソッド呼び出し
	GameObject::Draw();

}





void MenuUI::UpdateSelectedObject()
{
	if (Input::GetKeyTrigger(VK_RIGHT)) {
		// 右矢印キーが押された場合、選択中のオブジェクトを次に変更
		switch (m_SelectedObject) {
		case MenuObject::Item:
			m_SelectedObject = MenuObject::Quest;
			break;
		case MenuObject::Quest:
			m_SelectedObject = MenuObject::Information;
			break;
		case MenuObject::Information:
			// 椅子が選択されている場合は再び最初のオブジェクトに戻る
			m_SelectedObject = MenuObject::System;
			break;
		case MenuObject::System:
			// 椅子が選択されている場合は再び最初のオブジェクトに戻る
			m_SelectedObject = MenuObject::System;
			break;
		default:
			break;
		}
	}
	else if (Input::GetKeyTrigger(VK_LEFT)) {
		// 左矢印キーが押された場合、選択中のオブジェクトを前に変更
		switch (m_SelectedObject) {
		case MenuObject::Item:
			// 最初のオブジェクトの場合は最後のオブジェクトに移動
			m_SelectedObject = MenuObject::Item;
			break;
		case MenuObject::Quest:
			m_SelectedObject = MenuObject::Item;
			break;
		case MenuObject::Information:
			m_SelectedObject = MenuObject::Quest;
			break;
		case MenuObject::System:
			m_SelectedObject = MenuObject::Information;
			break;
		default:
			break;
		}
	}
}

