#pragma once
#include "gameobject.h"

class CPolygon :public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	//ここにシェーダー関連の変数を追加
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};