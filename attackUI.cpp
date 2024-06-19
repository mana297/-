#include "main.h"
#include "scene.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "sprite.h"
#include "audio.h"
#include "attackUI.h"

void AttackUI::Init()
{
	{
		compornent = new Sprite();
		compornent->Init(1000.0f, 500.0f, 150.0f, 150.0f, "asset\\texture\\AttackButton.png");
		compornent->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void AttackUI::UnInit()
{
	GameObject::UnInit();
	delete compornent;
}

void AttackUI::Update()
{
	GameObject::Update();

	// Player‚Ìó‘Ô‚ðŽæ“¾
	Player* player = Manager::GetScene()->GetGameObject<Player>();
	bool attackkey = player->GetAttackKey();

	if (attackkey)
	{
		compornent->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		compornent->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}



}

void AttackUI::Draw()
{
	GameObject::Draw();

	compornent->Draw();
}
