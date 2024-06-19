#pragma once
#include "compornent.h"

class TexUI :public Compornent{
private:
	class Sprite* menuUI_01;
	class Sprite* menuUI_02;
	class Sprite* menuUI_03;
	class Sprite* menuUI_04;

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();

};
