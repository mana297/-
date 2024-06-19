#pragma once
#include "scene.h"

class Title:public Scene
{
private:
	class Fade* m_Fade{};
	class Audio* m_BGM;
public:
	void Init()override;
	void Update()override;
};
