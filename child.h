#pragma once
#include "model.h"
#include "gameobject.h"

class Child :public GameObject {
private:
	Model* m_Model{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	class Shadow* m_Shadow{};

	class Player* m_Parent;

	//HP
	int m_HP = 100;

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();

	// HP‚ğŒ¸‚ç‚·ƒƒ\ƒbƒh‚ğ’Ç‰Á
	void DecreaseHP(int damage);

	void SetParent(Player* Parent) { m_Parent = Parent; }
};
