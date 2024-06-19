#pragma once
#include "gameobject.h"

enum class MenuObject {
	Item,
	Quest,
	Information,
	System,
};

class MenuUI :public GameObject {
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	// �ǂ̃I�u�W�F�N�g���I������Ă��邩�̃����o�ϐ�
	MenuObject m_SelectedObject;

	//���֌W
	class Audio* m_DropItemSE{};
	class Audio* m_StandItemSE{};
public:
	void Init();
	void UnInit();
	void Update();
	void Draw();


	void UpdateSelectedObject();

	MenuObject GetSelectObject()
	{
		return m_SelectedObject;
	}
};
