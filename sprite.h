#pragma once
#include "compornent.h"

class Sprite :public Compornent {
private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture{};

	//êFÇ∆Ç©ìßñæìxÇïœÇ¶ÇÈÇΩÇﬂÇÃÇ‡ÇÃ
	D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
public:
	virtual void Init(float x, float y, float Width, float Height, const char*TextureName);
	virtual void UnInit();
	virtual void Update();
	virtual void Draw();

	void SetColor(D3DXCOLOR color) { m_Color = color; }
	D3DXCOLOR GetColor() const { return m_Color; }
};