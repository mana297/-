#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "loading.h"
#include "loadinglogo.h"
#include "Input.h"
#include "game.h"
#include "fade.h"

#include <thread>

void Loading::Init()
{
	AddGameObject<Loadinglogo>(2);

	std::thread th(&Game::Load);
	th.detach();
}

void Loading::Update()
{
	Scene::Update();

	//ƒL[“ü—Í‚ÅƒQ[ƒ€‰æ–Ê‚Ö‘JˆÚ
	if (Game::GetLoadFinish())
	{
		//Game::Load();
		Manager::SetScene<Game>();
	}
}
