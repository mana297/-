#pragma once
#include <string>
#include "animationmodel.h"
#include "gameobject.h"

// エネミーの状態を示す変数を定義
enum EnemyState {
	STATE_STAND,
	STATE_NORMAL,
	STATE_CHASE_PLAYER,
	STATE_ATTACK
};

class Enemy :public GameObject {
private:
	//ステートパターン関連のメンバ変数
	EnemyState m_EnemyState = STATE_NORMAL;  // エネミーの初期状態を設定

	AnimationModel* m_Model{};

	//速度を管理
	D3DXVECTOR3 m_Velocity{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	class Shadow* m_Shadow{};

	//エネミーのHP
	int m_HP = 100;

	// エネミーの初期位置
	D3DXVECTOR3 m_InitialPosition{};

	//bool InitialPositionFlag = false; //初期位置かどうか

	D3DXMATRIX m_Matrix;

	//アニメーションを管理するためのタイム
	int m_Time{};
	float m_BlendRate{};
	std::string m_AnimationName;
	std::string m_NextAnimationName;

	//音関係
	class Audio* m_LookPlayer{};
	class Audio* m_AttackPlayer1 {};
	class Audio* m_AttackPlayer2{};
	class Audio* m_AttackPlayer3{};

	float m_AttackCooldown = 3.0f; // 攻撃のクールダウン時間
	float m_TimeSinceLastAttack; // 最後の攻撃からの経過時間
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

	// HPを減らすメソッドを追加
	void DecreaseHP(int damage);

	void ReturnToPosition(); //エネミーが初期位置に戻っていく
	void ChasePlayer(); //プレイヤーを追跡する
	void EnemyStand(); //エネミーのアイドル状態
	void AttackPlayer(); //プレイヤーを攻撃する
};
