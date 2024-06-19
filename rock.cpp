#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "rock.h"
#include "shadow.h"
#include "exprosion.h"

Model* Rock::m_Model{};

void Rock::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\CaveCrystal01.obj");
}

void Rock::UnLoad()
{
	m_Model->Unload();
	delete m_Model;
}

void Rock::Init()
{
	SetScale(D3DXVECTOR3(0.2f, 0.2f, 0.2f));

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

	m_Shadow = AddCompornent<Shadow>();
}

void Rock::UnInit()
{
	GameObject::UnInit();
	
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


}

void Rock::Update()
{
	
}

void Rock::Draw()
{
	GameObject::Draw();

	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	//�l���`�J�����O�i�J�����O�ɃI�u�W�F�N�g�����鎞�\�����Ȃ��j
	if (!camera->CheckView(m_Position))
	{
		return;
	}

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
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}

