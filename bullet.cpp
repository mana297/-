#include "main.h"
#include "renderer.h"
#include "bullet.h"
#include "scene.h"
#include "manager.h"
#include "audio.h"
#include "enemy.h"
#include "exprosion.h"
#include "score.h"

Model* Bullet::m_Model{};

void Bullet::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\torus.obj");
}

void Bullet::UnLoad()
{
	m_Model->Unload();
	delete m_Model;
}

void Bullet::Init()
{
	//�����̌�����ݒ�
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.1f);

	m_HitSE = AddCompornent<Audio>();
	m_HitSE->Load("asset\\audio\\SE\\attack.wav");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

}

void Bullet::UnInit()
{
	

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


}

void Bullet::Update()
{
	m_Position += m_Velocity;

	if (m_Position.z > 10.0f)
	{
		SetDestroy();
	}

	//�G�Ƃ̏Փ˔���
	Scene* scene = Manager::GetScene();
	std::vector<Enemy*> enemies = scene->GetGameObjects<Enemy>();

	for (Enemy* enemy : enemies)
	{
		D3DXVECTOR3 enemyPos = enemy->GetPosition();
		D3DXVECTOR3 direction = enemyPos - m_Position;
		float length = D3DXVec3Length(&direction);


		//�G�l�~�[�ƒe���������Ă���̏���
		if (length < 2.0f)
		{
			//�G�Ƀ_���[�W��^�����Ƃ��ɗ����G�t�F�N�g
			scene->AddGameObject<Exprosion>(1)->SetPosition(enemyPos);

			//�G�ɋ����q�b�g�����Ƃ��ɗ����SE
			m_HitSE->Play();

			// �e�̈З͂����G�l�~�[��HP�����炷
			enemy->DecreaseHP(m_Power);

			//�e�̔j�󏈗�
			SetDestroy();

			Score* score = scene->GetGameObject<Score>();
			score->AddCount(1);

			return;
		}
	}
}

void Bullet::Draw()
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���b�N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x*0.3f, m_Scale.y*0.3f, m_Scale.z*0.3f);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}
