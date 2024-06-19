#pragma once
#include "scene.h"

class Game :public Scene
{

private:
	static bool m_LoadFinish;
public:
	//�t���C�E�F�C�g�p�^�[��
	static void Load();
	static void UnLoad();
	static bool GetLoadFinish() { return m_LoadFinish; }

	void Init()override;
	void UnInit()override;
	void Update()override;
};