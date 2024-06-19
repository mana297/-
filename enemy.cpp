#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include <ctime>
#include "enemy.h"
#include "player.h"
#include "shadow.h"
#include "exprosion.h"
#include "cylinder.h"
#include "box.h"
#include "rock.h"
#include "meshField.h"
#include "Time.h"
#include "audio.h"

void Enemy::Init()
{
	//�A�j���[�V�����̓ǂݍ���
	m_Model = new AnimationModel();
	m_Model->Load("asset\\enemy\\E_Vampire.fbx");
	m_Model->LoadAnimation("asset\\model\\Bot_Run.fbx", "Run");
	m_Model->LoadAnimation("asset\\model\\Walking.fbx", "Walk");
	m_Model->LoadAnimation("asset\\enemy\\Idle.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\Jumping.fbx", "Jump");

	//�ŏ��̓A�C�h����Ԃɂ���
	m_AnimationName = "Idle";
	m_NextAnimationName = "Idle";

	m_Scale = D3DXVECTOR3(0.018f, 0.018f, 0.018f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

	m_LookPlayer = AddCompornent<Audio>();
	m_LookPlayer->Load("asset\\audio\\ENEMY\\02_monster_before.wav");

	//�U���֌W�̃{�C�X
	m_AttackPlayer1 = AddCompornent<Audio>();
	m_AttackPlayer1->Load("asset\\audio\\ENEMY\\07_monster_attack.wav");
	m_AttackPlayer2 = AddCompornent<Audio>();
	m_AttackPlayer2->Load("asset\\audio\\ENEMY\\09_monster_attack.wav");
	m_AttackPlayer3 = AddCompornent<Audio>();
	m_AttackPlayer3->Load("asset\\audio\\ENEMY\\11_monster_attack.wav");

	m_Shadow = AddCompornent<Shadow>();

	// ���������Ƀ����_���V�[�h��ݒ�
	srand(static_cast<unsigned>(time(NULL)));
}

void Enemy::UnInit()
{
	GameObject::UnInit();
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


}

void Enemy::Update()
{
	GameObject::Update();

	Scene* scene = Manager::GetScene();

	// �v���C���[�̈ʒu�����擾
	D3DXVECTOR3 playerPosition = Manager::GetScene()->GetGameObject<Player>()->GetPosition();

	// �G�l�~�[�̏�Ԃɉ����ď����𕪊�
	switch (m_EnemyState)
	{
	case STATE_STAND:
		// STATE_STAND �̏ꍇ�A�A�C�h����ԏ������s��
		EnemyStand();
		break;
	case STATE_NORMAL:
		// STATE_NORMAL �̏ꍇ�A�����ʒu�ɖ߂鏈�����s��
		ReturnToPosition();
		break;
	case STATE_CHASE_PLAYER:
		// STATE_CHASE_PLAYER �̏ꍇ�A�v���C���[��ǐՂ��鏈�����s��
		ChasePlayer();
		break;
	case STATE_ATTACK:
		// STATE_ATTACK �̏ꍇ�A�v���C���[���U�����鏈�����s��
		AttackPlayer();
		break;
	default:
		break;
		// ���̃G�l�~�[�̏�ԂɊւ��鏈��...
	}


	float groundHeight = 0.0f;  // �f�t�H���g�̒n�ʍ�����ݒ�

	// MeshField���猻�݈ʒu�̒n�ʍ������擾����R�[�h��ǉ�
	MeshField* meshField = Manager::GetScene()->GetGameObject<MeshField>();
	groundHeight = meshField->GetHight(m_Position);

	// �G�l�~�[�̈ʒu��n�ʂ̍����ɐ���
	if (m_Position.y < groundHeight)
	{
		m_Position.y = groundHeight;
	}




	// �v���C���[�ƃG�l�~�[�̋������v�Z
	D3DXVECTOR3 distanceVector = playerPosition - m_Position;
	float distanceToPlayer = D3DXVec3Length(&distanceVector);

	// ���̋����ȉ��Ƀv���C���[�����邩����
	if (distanceToPlayer < 10.0f) {//�G�l�~�[���ǂ̂��炢�̋����ɂȂ�����ǐՂ��邩�̋���
		m_EnemyState = STATE_CHASE_PLAYER;

		// �G�l�~�[�ƃv���C���[�̋�����5�����̏ꍇ�A�U�������Ɉڍs
		if (distanceToPlayer < 10.0f) {
			// �����ɍU�����������s����R�[�h��ǉ�
			// �N�[���_�E�����Ԃ�i�߂�
			m_TimeSinceLastAttack += Time::GetDeltaTime(); // Time::GetDeltaTime() �͌o�ߎ��Ԃ�Ԃ��֐�

			if (m_TimeSinceLastAttack >= m_AttackCooldown) {
				// �N�[���_�E�����I��������U�������s
				/*
				�����������A�^�b�N�v���C���[�ł͂Ȃ��o���b�g�Ƃ��Ăяo����
				�ʔ��˂Ȃǂł���悤�ɂȂ�
				����ł��̋ʔ��˂̒��Ƀv���C���[�ɓ���������A�^�b�N�v���C���[�����s����΂��������ɂȂ�Ǝv��
				*/
				AttackPlayer();
				m_TimeSinceLastAttack = 0.0f; // �U����A�N�[���_�E���̃��Z�b�g
			}
		}
	}
	else if (m_EnemyState != STATE_CHASE_PLAYER) {
		// ���̋����ȏ�Ƀv���C���[������ꍇ�A�G�l�~�[�̏�Ԃ�ʏ탂�[�h�ɖ߂�
		m_EnemyState = STATE_NORMAL;
	}
	else
	{
		m_EnemyState = STATE_STAND;
	}



	//�G�l�~�[�̒����ɉe��\��
	D3DXVECTOR3 shadowPosition = m_Position;
	shadowPosition.y = groundHeight + 0.01f;
	m_Shadow->SetPosition(shadowPosition);

	//�G�l�~�[�̃A�j���[�V��������
	UpdateAnimation();
}

void Enemy::Draw()
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
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y + D3DX_PI, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	m_Matrix = world;

	Renderer::SetWorldMatrix(&world);

	
	m_Model->Draw();
}

void Enemy::UpdateAnimation()
{
	m_Model->Update(m_AnimationName.c_str(), m_Time, m_NextAnimationName.c_str(), m_Time,
		m_BlendRate);
	m_Time++;
	m_BlendRate += 0.05f;//�����ŃA�j���[�V�����u�����h�̑����𒲐�
	if (m_BlendRate > 1.0f)
		m_BlendRate = 1.0f;
}

void Enemy::DecreaseHP(int damage)
{
	// �e�̈З͕����� HP �����炷
	m_HP -= damage; //Bullet�ňЗ͏����Ă��

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

void Enemy::ReturnToPosition()
{
	// ���݂̃A�j���[�V�������uWalk�v�łȂ��ꍇ�́A�uWalk�v�ɐݒ�
/*	if (m_AnimationName != "Walk") {
		m_AnimationName = "Walk";
		m_NextAnimationName = "Walk";
		m_BlendRate = 0.0f;
	}*/

	// �G�l�~�[�̌��݈ʒu���珉���ʒu�܂ł̋������v�Z
	D3DXVECTOR3 distanceVector = m_InitialPosition - m_Position;
	float distanceToInitialPosition = D3DXVec3Length(&distanceVector);

	// ���̈ړ����x�ŏ����ʒu�Ɍ������Ĉړ�
	float moveSpeed = 0.1f; // �ړ����x�𒲐�

	if (distanceToInitialPosition > 0.1f) // ������x�̋���������ꍇ�݈̂ړ�
	{
		// �ړ��x�N�g�����v�Z
		D3DXVECTOR3 moveDirection = distanceVector / distanceToInitialPosition;

		// �G�l�~�[���ړ�
		m_Position += moveDirection * moveSpeed;
	}
}

void Enemy::ChasePlayer()
{
	// ���݂̃A�j���[�V�������uWalk�v�łȂ��ꍇ�́A�uWalk�v�ɐݒ�
	if (m_AnimationName != "Walk") {
		m_AnimationName = "Walk";
		m_NextAnimationName = "Walk";
		m_BlendRate = 0.0f;
	}
	// ���̋����ȉ��Ƀv���C���[������ꍇ�A�G�l�~�[�̏�Ԃ�ǐՃ��[�h�ɕύX
	//m_LookPlayer->Play(); // �v���C���[�����������̉����Đ�

	// �v���C���[�̈ʒu�����擾
	D3DXVECTOR3 playerPosition = Manager::GetScene()->GetGameObject<Player>()->GetPosition();

	// �v���C���[�ւ̕����x�N�g�����v�Z
	D3DXVECTOR3 directionToPlayer = playerPosition - m_Position;
	D3DXVec3Normalize(&directionToPlayer, &directionToPlayer);

	// �G�l�~�[�̌�������]�s�񂩂�擾
	D3DXVECTOR3 enemyForward = directionToPlayer; // �v���C���[�̕����x�N�g���Ɠ���

	// �G�l�~�[�̈ʒu���X�V
	m_Position += enemyForward * 0.05f; // �G�l�~�[�̑��x
}

void Enemy::EnemyStand()
{
	// ���݂̃A�j���[�V�������uIdle�v�łȂ��ꍇ�́A�uIdle�v�ɐݒ�
	if (m_AnimationName != "Idle") {
		m_AnimationName = "Idle";
		m_NextAnimationName = "Idle";
		m_BlendRate = 0.0f;
	}
}

// �G�l�~�[���v���C���[���U������֐�
void Enemy::AttackPlayer() {
	// �v���C���[�̃N���X�ɃA�N�Z�X���邽�߂̃|�C���^���擾
	Player* player = Manager::GetScene()->GetGameObject<Player>();

	// �v���C���[�̈ʒu���擾
	D3DXVECTOR3 playerPosition = player->GetPosition();

	// �v���C���[�Ƀ_���[�W��^����
	player->TakeDamage();

	//�U�����̃{�C�X
	int randomVoice = rand() % 3; // Init�֐��ɗp�ӂ����U���{�C�X�̐�
	if (randomVoice == 0) {
		m_AttackPlayer1->Play(); // �{�C�X1���Đ�
	}
	else if (randomVoice == 1) {
		m_AttackPlayer2->Play(); // �{�C�X2���Đ�
	}
	else {
		m_AttackPlayer3->Play(); // �{�C�X3���Đ�
	}

	// �����ōU���G�t�F�N�g���v���C���[�̈ʒu�ɕ\������
	Scene* scene = Manager::GetScene();
	scene->AddGameObject<Exprosion>(1)->SetPosition(playerPosition);
}
