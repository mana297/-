#include "main.h"
#include "renderer.h"
#include "Polygon2D.h"
#include "sprite.h"

void Polygon2D::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	//2Dテクスチャの表示
	//Sprite* sprite = AddCompornent<Sprite>();
	//sprite->Init(100.0f, 100.0f, 300.0f, 200.0f, "asset\\texture\\field001.jpg");

	//これも上と同じ
	AddCompornent<Sprite>()->Init(200.0f, 200.0f, 200.0f, 200.0f, "asset\\texture\\eddie.png");
	//AddCompornent<Sprite>()->Init(400.0f, 100.0f, 300.0f, 200.0f, "asset\\texture\\diluc.png");
	//AddCompornent<Sprite>()->Init(700.0f, 100.0f, 300.0f, 200.0f, "asset\\texture\\diluc.png");

}

void Polygon2D::UnInit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	//基底クラスのメソッド呼び出し
	GameObject::UnInit();
}

void Polygon2D::Update()
{
	//基底クラスのメソッド呼び出し
	GameObject::Update();
}

void Polygon2D::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリックス設定
	Renderer::SetWorldViewProjection2D();

	//基底クラスのメソッド呼び出し
	GameObject::Draw();
}
