#pragma once
#include <string>
#include "animationmodel.h"
#include "gameobject.h"

// �G�l�~�[�̏�Ԃ������ϐ����`
enum EnemyState {
	STATE_STAND,
	STATE_NORMAL,
	STATE_CHASE_PLAYER,
	STATE_ATTACK
};

class Enemy :public GameObject {
private:
	//�X�e�[�g�p�^�[���֘A�̃����o�ϐ�
	EnemyState m_EnemyState = STATE_NORMAL;  // �G�l�~�[�̏�����Ԃ�ݒ�

	AnimationModel* m_Model{};

	//���x���Ǘ�
	D3DXVECTOR3 m_Velocity{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	class Shadow* m_Shadow{};

	//�G�l�~�[��HP
	int m_HP = 100;

	// �G�l�~�[�̏����ʒu
	D3DXVECTOR3 m_InitialPosition{};

	//bool InitialPositionFlag = false; //�����ʒu���ǂ���

	D3DXMATRIX m_Matrix;

	//�A�j���[�V�������Ǘ����邽�߂̃^�C��
	int m_Time{};
	float m_BlendRate{};
	std::string m_AnimationName;
	std::string m_NextAnimationName;

	//���֌W
	class Audio* m_LookPlayer{};
	class Audio* m_AttackPlayer1 {};
	class Audio* m_AttackPlayer2{};
	class Audio* m_AttackPlayer3{};

	float m_AttackCooldown = 3.0f; // �U���̃N�[���_�E������
	float m_TimeSinceLastAttack; // �Ō�̍U������̌o�ߎ���
public:
	void Init();
	void UnInit();
	void Update();
	void Draw();
	void UpdateAnimation();

	void SetInitialPosition(D3DXVECTOR3 position) 
	{ 
		m_InitialPosition = position; 
		SetPosition(m_InitialPosition);
	}

	// HP�����炷���\�b�h��ǉ�
	void DecreaseHP(int damage);

	void ReturnToPosition(); //�G�l�~�[�������ʒu�ɖ߂��Ă���
	void ChasePlayer(); //�v���C���[��ǐՂ���
	void EnemyStand(); //�G�l�~�[�̃A�C�h�����
	void AttackPlayer(); //�v���C���[���U������
};
