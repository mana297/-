#pragma once
#include "compornent.h"
#include <list>
//
//現段階は・・・・・・
//
//カメラ、フィールド、ポリゴン2D、プレイヤーの継承元

class GameObject {
protected:
	bool m_Destroy = false;

	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	std::list<Compornent*> m_Compornent;
public:
	//仮想関数
	virtual void Init() {};

	virtual void UnInit() 
	{
		//範囲forループ
		for (Compornent* compornent : m_Compornent)
		{
			compornent->UnInit();
			delete compornent;
		}
		//空になったリスト構造の入れ物を削除する
		m_Compornent.clear();
	};
	virtual void Update() 
	{
		//範囲forループ
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

	template <typename T>//テンプレート関数
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

	//前方向きベクトル取得
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

	//右向きベクトル取得
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
