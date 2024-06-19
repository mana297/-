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

	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���b�N�X�ݒ�
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
	// �e�̈З͕����� HP �����炷
	m_HP -= damage;

	// HP��0�ȉ��ɂȂ�����G�l�~�[��j�󂷂�
	if (m_HP <= 0)
	{
		// �G�l�~�[��HP��0�ȉ��ɂȂ����甚���G�t�F�N�g�𐶐����Ĉʒu��ݒ�
		Scene* scene = Manager::GetScene();
		scene->AddGameObject<Exprosion>(1)->SetPosition(GetPosition());

		SetDestroy();
		// ���̏���...
	}
}

