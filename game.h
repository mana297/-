#pragma once
#include "scene.h"

class Game :public Scene
{

private:
	static bool m_LoadFinish;
public:
	//フライウェイトパターン
	static void Load();
	static void UnLoad();
	static bool GetLoadFinish() { return m_LoadFinish; }

	void Init()override;
	void UnInit()override;
	void Update()override;
};