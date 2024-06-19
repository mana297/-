#pragma once
#include "gameobject.h"

class Resultlogo :public GameObject {
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();
};
