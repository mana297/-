#pragma once
#include "model.h"
#include "gameobject.h"

class Bullet :public GameObject {
private:
	D3DXVECTOR3 m_Velocity{};

	//�t���C�E�F�C�g�p�^�[��
	static Model* m_Model;

	class Audio* m_HitSE{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	//�e�̈З�
	int m_Power = 50;
public:
	//�t���C�E�F�C�g�p�^�[��
	static void Load();
	static void UnLoad();

	void Init();
	void UnInit();
	void Update();
	void Draw();

	void SetVelocity(D3DXVECTOR3 velocity) { m_Velocity = velocity; }
};
