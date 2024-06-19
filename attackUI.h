#pragma once
#include "gameobject.h"

class AttackUI :public GameObject {
private:
	class Sprite* compornent;

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();
};
