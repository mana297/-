#pragma once
#include "model.h"
#include "gameobject.h"

class Tree :public GameObject {
private:
	//�t���C�E�F�C�g�p�^�[��
	static Model* m_Model;

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	class Shadow* m_Shadow{};

public:
	//�t���C�E�F�C�g�p�^�[��
	static void Load();
	static void UnLoad();

	void Init();
	void UnInit();
	void Update();
	void Draw();

};
