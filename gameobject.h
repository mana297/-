#pragma once
#include "compornent.h"
#include <list>
//
//���i�K�́E�E�E�E�E�E
//
//�J�����A�t�B�[���h�A�|���S��2D�A�v���C���[�̌p����

class GameObject {
protected:
	bool m_Destroy = false;

	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	std::list<Compornent*> m_Compornent;
public:
	//���z�֐�
	virtual void Init() {};

	virtual void UnInit() 
	{
		//�͈�for���[�v
		for (Compornent* compornent : m_Compornent)
		{
			compornent->UnInit();
			delete compornent;
		}
		//��ɂȂ������X�g�\���̓��ꕨ���폜����
		m_Compornent.clear();
	};
	virtual void Update() 
	{
		//�͈�for���[�v
		for (Compornent* compornent : m_Compornent)
		{
			compornent->Update();
		}
	};
	virtual void Draw() 
	{
		for (Compornent* compornent : m_Compornent)
		{
			compornent->Draw();
		}
	};

	template <typename T>//�e���v���[�g�֐�
	T* AddCompornent()
	{
		Compornent* compornent = new T;
		m_Compornent.push_back(compornent);
		compornent->Init();

		return (T*)compornent;
	}

	void SetPosition(D3DXVECTOR3 Position) 
	{
		m_Position = Position;
	}

	void SetScale(D3DXVECTOR3 Scale)
	{
		m_Scale = Scale;
	}

	D3DXVECTOR3 GetPosition()
	{
		return m_Position;
	}

	D3DXVECTOR3 GetScale() { return m_Scale; }

	//�O�������x�N�g���擾
	D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	//�E�����x�N�g���擾
	D3DXVECTOR3 GetRight()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._11;
		forward.y = rot._12;
		forward.z = rot._13;

		return forward;
	}

	void SetDestroy()
	{
		m_Destroy = true;
	}

	bool Destroy()
	{
		if (m_Destroy)
		{
			UnInit();
			delete this;
			return true;
		}
		else 
		{
			return false;
		}
	}
};
