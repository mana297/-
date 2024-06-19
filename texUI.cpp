#include "main.h"
#include "scene.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "sprite.h"
#include "audio.h"
#include "texUI.h"
#include "menuUI.h"

void TexUI::Init()
{
	//ここからメニューUIのテクスチャ
	{
		Sprite* itemTex = new Sprite();

		itemTex->Init(140.0f, 100.0f, 150.0f, 50.0f, "asset\\texture\\itemUI.png");
		itemTex->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		menuUI_01 = itemTex;
	}

	{
		Sprite* questTex = new Sprite();

		questTex->Init(340.0f, 100.0f, 150.0f, 50.0f, "asset\\texture\\questUI.png");
		questTex->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		menuUI_02 = questTex;
	}

	{
		Sprite* informationTex = new Sprite();

		informationTex->Init(540.0f, 100.0f, 150.0f, 50.0f, "asset\\texture\\informationUI.png");
		informationTex->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

		menuUI_03 = informationTex;
	}

	{
		Sprite* systemTex = new Sprite();

		systemTex->Init(740.0f, 100.0f, 150.0f, 50.0f, "asset\\texture\\systemUI.png");
		systemTex->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

		menuUI_04 = systemTex;
	}
}

void TexUI::UnInit()
{
	delete menuUI_01;
	delete menuUI_02;
	delete menuUI_03;
	delete menuUI_04;

}

void TexUI::Update()
{
	MenuUI* menuUI = Manager::GetScene()->GetGameObject<MenuUI>();

	if (menuUI)
	{
		MenuObject menuobject = menuUI->GetSelectObject();

		// 選択されているオブジェクトに応じて表示する画像を決定
		switch (menuobject)
		{
		case MenuObject::Item:
			menuUI_01->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			menuUI_02->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			menuUI_03->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			menuUI_04->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			break;
		case MenuObject::Quest:
			menuUI_01->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			menuUI_02->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			menuUI_03->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			menuUI_04->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));			
			break;
		case MenuObject::Information:
			menuUI_01->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			menuUI_02->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			menuUI_03->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			menuUI_04->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));			
			break;
		case MenuObject::System:
			menuUI_01->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			menuUI_02->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			menuUI_03->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			menuUI_04->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));			
			break;
		default:
			// 何もしない（選択されていない場合は何も表示しない）
			break;
		}
	}
}

void TexUI::Draw()
{
	menuUI_01->Draw();
	menuUI_02->Draw();
	menuUI_03->Draw();
	menuUI_04->Draw();
}
