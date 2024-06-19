#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "child.h"
#include "shadow.h"
#include "exprosion.h"
#include "player.h"

void Child::Init()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\torus.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

	m_Shadow = AddCompornent<Shadow>();
}

void Child::UnInit()
{
	GameObject::UnInit();
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


}

void Child::Update()
{
	GameObject::Update();

	float groundHeight = 0.0f;

	D3DXVECTOR3 shadowPosition = m_Position;
	shadowPosition.y = groundHeight + 0.01f;
	m_Shadow->SetPosition(shadowPosition);
}

void Child::Draw()
{
	GameObject::Draw();

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリックス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans * m_Parent->GetMatrix();
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}

void Child::DecreaseHP(int damage)
{
	// 弾の威力分だけ HP を減らす
	m_HP -= damage;

	// HPが0以下になったらエネミーを破壊する
	if (m_HP <= 0)
	{
		// エネミーのHPが0以下になったら爆発エフェクトを生成して位置を設定
		Scene* scene = Manager::GetScene();
		scene->AddGameObject<Exprosion>(1)->SetPosition(GetPosition());

		SetDestroy();
		// 他の処理...
	}
}

