#include "main.h"
#include "renderer.h"
#include "loadinglogo.h"
#include "sprite.h"

void Loadinglogo::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	
	//これも上と同じ
	AddCompornent<Sprite>()->Init(0.0f, 0.0f, 300.0f, 300.0f, "asset\\texture\\exprosion.png");
	//AddCompornent<Sprite>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\diluc.png");
	//AddCompornent<Sprite>()->Init(400.0f, 100.0f, 300.0f, 200.0f, "asset\\texture\\diluc.png");
	//AddCompornent<Sprite>()->Init(700.0f, 100.0f, 300.0f, 200.0f, "asset\\texture\\diluc.png");

}

void Loadinglogo::UnInit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	//基底クラスのメソッド呼び出し
	GameObject::UnInit();
}

void Loadinglogo::Update()
{
	//基底クラスのメソッド呼び出し
	GameObject::Update();

	//Initで設定した座標の半分くらいに設定すると中心で回転する
	m_Position = D3DXVECTOR3(150.0f, 150.0f, 0.0f);
	m_Rotation.z += 0.1f;
}

void Loadinglogo::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリックス設定
	Renderer::SetWorldViewProjection2D();

	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	//基底クラスのメソッド呼び出し
	GameObject::Draw();
}
