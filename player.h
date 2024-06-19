#pragma once
#include <string>
#include "animationmodel.h"
#include "gameobject.h"
#include "exNum.h"

class MenuUI;

enum PLAYER_STATE
{
	PLAYER_STATE_GROUND,
	PLAYER_STATE_JUMP,
	PLAYER_MENU_UI, //�v���C���[�����j���[���J���Ă�����
};

class Player:public GameObject {
private:
	//�X�e�[�g�p�^�[���֘A�̃����o�ϐ�
	PLAYER_STATE m_PlayerState;
	
	//�v���C���[�X�e�[�^�X�p
	ExNum hp;// ExNum�^�̃����o�[�ϐ���錾

	class Fade* m_Fade{};

	AnimationModel* m_Model{};

	//�v���C���[�̑��x���Ǘ�
	D3DXVECTOR3 m_Velocity{};

	ID3D11VertexShader*m_VertexShader{};
	ID3D11PixelShader*m_PixelShader{};
	ID3D11InputLayout*m_VertexLayout{};

	class Audio* m_ShotSE{};
	class Shadow* m_Shadow{};

	bool jumpFlag = true; //�W�����v�����ǂ���

	float m_DashSpeed = 2.0f; // �_�b�V�����x�{��

	D3DXMATRIX m_Matrix;

	//�A�j���[�V�������Ǘ����邽�߂̃^�C��
	int m_Time{};
	float m_BlendRate{};
	std::string m_AnimationName;
	std::string m_NextAnimationName;

	//�ڒn���Ă��邩�̃t���O
	bool m_IsGround{};
	
	// �d���U���̂��߂̕ϐ�
	bool m_ChargingAttack = false;
    float m_ChargeTimer = 0.0f;
	const float m_ChargeDuration = 1.0f;
	bool m_SpaceKeyReleased = true; // �X�y�[�X�L�[�������ꂽ��Ԃ�ǐ�
	bool m_Attack = false;//�U����Ԃ��̃t���O

	//���j���[UI�̃t���O
	bool m_MenuUIFlag = false;

	//���j���[UI���Ăяo�����߂̕ϐ�
	MenuUI* m_MenuUI{};
	
public:
	// �R���X�g���N�^�̐錾
	Player() : hp(0, 100, 100) {}

	void LoadGame(); //�Z�[�u�f�[�^�����[�h���邽�߂̊֐�

	void Init();
	void UnInit();
	void Update();
	void Draw();
	void UpdateAnimation();
	//�X�e�[�g�p�^�[���֘A
	void UpdateGround();
	void UpdateJump();
	void UpdateMenuUI();
	// �_���[�W���󂯂�֐�
	void TakeDamage();

	bool GetAttackKey()
	{
		return m_Attack;
	}

	void SaveGame();//�Z�[�u���邽�߂̊֐�

	D3DXMATRIX GetMatrix()
	{
		return m_Matrix; 
	}

	bool GetMenuUIFlag() 
	{ 
		return m_MenuUIFlag; 
	}

	bool MenuUIActive() const 
	{
		return m_MenuUIFlag;
	}
};
