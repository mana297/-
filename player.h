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
	PLAYER_MENU_UI, //プレイヤーがメニューを開いている状態
};

class Player:public GameObject {
private:
	//ステートパターン関連のメンバ変数
	PLAYER_STATE m_PlayerState;
	
	//プレイヤーステータス用
	ExNum hp;// ExNum型のメンバー変数を宣言

	class Fade* m_Fade{};

	AnimationModel* m_Model{};

	//プレイヤーの速度を管理
	D3DXVECTOR3 m_Velocity{};

	ID3D11VertexShader*m_VertexShader{};
	ID3D11PixelShader*m_PixelShader{};
	ID3D11InputLayout*m_VertexLayout{};

	class Audio* m_ShotSE{};
	class Shadow* m_Shadow{};

	bool jumpFlag = true; //ジャンプ中かどうか

	float m_DashSpeed = 2.0f; // ダッシュ速度倍率

	D3DXMATRIX m_Matrix;

	//アニメーションを管理するためのタイム
	int m_Time{};
	float m_BlendRate{};
	std::string m_AnimationName;
	std::string m_NextAnimationName;

	//接地しているかのフラグ
	bool m_IsGround{};
	
	// 重撃攻撃のための変数
	bool m_ChargingAttack = false;
    float m_ChargeTimer = 0.0f;
	const float m_ChargeDuration = 1.0f;
	bool m_SpaceKeyReleased = true; // スペースキーが離された状態を追跡
	bool m_Attack = false;//攻撃常態かのフラグ

	//メニューUIのフラグ
	bool m_MenuUIFlag = false;

	//メニューUIを呼び出すための変数
	MenuUI* m_MenuUI{};
	
public:
	// コンストラクタの宣言
	Player() : hp(0, 100, 100) {}

	void LoadGame(); //セーブデータをロードするための関数

	void Init();
	void UnInit();
	void Update();
	void Draw();
	void UpdateAnimation();
	//ステートパターン関連
	void UpdateGround();
	void UpdateJump();
	void UpdateMenuUI();
	// ダメージを受ける関数
	void TakeDamage();

	bool GetAttackKey()
	{
		return m_Attack;
	}

	void SaveGame();//セーブするための関数

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
