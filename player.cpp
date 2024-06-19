#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "bullet.h"
#include "scene.h"
#include "cylinder.h"
#include "box.h"
#include "rock.h"
#include "meshField.h"
#include "audio.h"
#include "result.h"
#include "fade.h"
#include "Time.h"
#include "shadow.h"
#include "exprosion.h"
#include "sprite.h"
#include "attackUI.h"
#include "menuUI.h"


#include <iostream>
#include <fstream>

// プレイヤーの位置情報をロードする関数
void Player::LoadGame()
{
	std::ifstream file("saved_game.bin", std::ios::binary);
	if (file.is_open()) {
		// プレイヤーの位置情報を読み取る
		file.read(reinterpret_cast<char*>(&m_Position), sizeof(m_Position));

		// 他のプレイヤーステータス情報を読み取る
		file.read(reinterpret_cast<char*>(&m_PlayerState), sizeof(m_PlayerState));
		file.read(reinterpret_cast<char*>(&hp), sizeof(hp));
		file.read(reinterpret_cast<char*>(&m_Velocity), sizeof(m_Velocity));
		file.read(reinterpret_cast<char*>(&jumpFlag), sizeof(jumpFlag));
		file.read(reinterpret_cast<char*>(&m_DashSpeed), sizeof(m_DashSpeed));
		file.read(reinterpret_cast<char*>(&m_Time), sizeof(m_Time));
		file.read(reinterpret_cast<char*>(&m_BlendRate), sizeof(m_BlendRate));
		file.read(reinterpret_cast<char*>(&m_IsGround), sizeof(m_IsGround));
		file.read(reinterpret_cast<char*>(&m_ChargingAttack), sizeof(m_ChargingAttack));
		file.read(reinterpret_cast<char*>(&m_ChargeTimer), sizeof(m_ChargeTimer));
		file.read(reinterpret_cast<char*>(&m_SpaceKeyReleased), sizeof(m_SpaceKeyReleased));
		file.read(reinterpret_cast<char*>(&m_Attack), sizeof(m_Attack));
		file.read(reinterpret_cast<char*>(&m_MenuUIFlag), sizeof(m_MenuUIFlag));

		file.close();
		std::cout << "Game loaded successfully." << std::endl;
	}
	else {
		std::cout << "Failed to load the game." << std::endl;
	}
}

void Player::Init()
{
	//アニメーションの読み込み
	m_Model = new AnimationModel();

	m_Model->Load("asset\\model\\Erika Archer.fbx");
	m_Model->LoadAnimation("asset\\model\\Bot_Run.fbx", "Run");
	m_Model->LoadAnimation("asset\\model\\Walking.fbx", "Walk");
	m_Model->LoadAnimation("asset\\model\\Bot_Idle.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\Jumping.fbx", "Jump");

	//最初はアイドル状態にする
	m_AnimationName = "Idle";
	m_NextAnimationName = "Idle";

	m_Scale = D3DXVECTOR3(0.02f, 0.02f, 0.02f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\depthShadowMappingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\depthShadowMappingPS.cso");

	m_ShotSE = AddCompornent<Audio>();
	m_ShotSE->Load("asset\\audio\\SE\\wan.wav");

}

void Player::UnInit()
{
	GameObject::UnInit();
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Player::Update()
{
	GameObject::Update();

	Scene* scene = Manager::GetScene();

	//プレイヤーの過去の位置を保存
	D3DXVECTOR3 oldPosition = m_Position;

	//ステートパターン
	switch (m_PlayerState)
	{
	case PLAYER_STATE_GROUND:
		UpdateGround();
		break;
	case PLAYER_STATE_JUMP:
		UpdateJump();
		break;
	case PLAYER_MENU_UI:
		UpdateMenuUI();
		break;
	default:
		break;
	}

	// F1キーが押されたらクイックセーブ処理を実行
	if (Input::GetKeyTrigger(VK_F1))
	{
		// ゲームをセーブする
		SaveGame();
	}

	//重力
	m_Velocity.y -= 0.015f;
	//移動
	m_Position += m_Velocity;

	//障害物判定
	float groundHeight;

	MeshField* meshField = scene->GetGameObject<MeshField>();
	groundHeight = meshField->GetHight(m_Position);

	//円柱
	std::vector<Cylinder*> cylinders = scene->GetGameObjects<Cylinder>();

	for (Cylinder* cylinder : cylinders)
	{
		D3DXVECTOR3 position = cylinder->GetPosition();
		D3DXVECTOR3 scale = cylinder->GetScale();

		D3DXVECTOR3 direction = m_Position - position;
		direction.y = 0.0f;

		float length = D3DXVec3Length(&direction);



		if (length < scale.x)
		{
			if (m_Position.y < position.y + scale.y - 0.5f)
			{
				m_Position.x = oldPosition.x;
				m_Position.z = oldPosition.z;
			}
			else
			{
				groundHeight = position.y + scale.y;
			}
			break;
		}
	}

	//直方体
	std::vector<Box*> boxes = scene->GetGameObjects<Box>();

	for (Box* box : boxes)
	{
		D3DXVECTOR3 position = box->GetPosition();
		D3DXVECTOR3 scale = box->GetScale();

		if (position.x - scale.x - 0.5f < m_Position.x &&
			m_Position.x < position.x + scale.x + 0.5f &&
			position.z - scale.z - 0.5f < m_Position.z &&
			m_Position.z < position.z + scale.z + 0.5f)
		{
			if (m_Position.y < position.y + scale.y * 2.0f - 0.5f)
			{
				m_Position.x = oldPosition.x;
				m_Position.z = oldPosition.z;
			}
			else
			{
				groundHeight = position.y + scale.y * 2.0f;
			}
		}
	}

	//岩オブジェクト
	std::vector<Rock*> rocks = scene->GetGameObjects<Rock>();

	//岩オブジェクトに接触しているかどうかを示すフラグ
	bool isClimbing = false;

	for (Rock* rock : rocks)
	{
		D3DXVECTOR3 position = rock->GetPosition();
		D3DXVECTOR3 scale = rock->GetScale();

		if (position.x - scale.x - 6.5f < m_Position.x &&
			m_Position.x < position.x + scale.x + 6.5f &&
			position.z - scale.z - 2.7f < m_Position.z &&
			m_Position.z < position.z + scale.z + 2.7f)
		{
			if (m_Position.y < position.y + scale.y * 2.0f - 0.5f)
			{
				m_Position.x = oldPosition.x;
				m_Position.z = oldPosition.z;
				m_Position.y = oldPosition.y;

				// 岩オブジェクトに接触したら、しがみつく
				isClimbing = true;
				break;
			}
			else
			{
				isClimbing = false;
				groundHeight = position.y + scale.y * 2.0f;
			}
		}
	}

	if (isClimbing)
	{
		// 壁のぼり中の処理をここに追加
		// 特定のキー入力に応じて上昇させるなどの処理を追加する
		if (Input::GetKeyPress('W'))
		{
			// Wキーが押されている場合、上昇させる処理を追加
			m_Position.y += 0.1f; // 上昇速度を調整
		}

		if (Input::GetKeyPress('J'))
		{
			// Jキーが押されている場合、上昇させる処理を追加
			m_Position.y += 0.15f; // 上昇速度を調整
		}
	}


	//接地
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_IsGround = true;

		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;

		jumpFlag = true;
	}
	else
	{
		m_IsGround = false;
	}


	// スペースキーが押されている間、充電攻撃を準備
	if (Input::GetKeyPress(VK_SPACE)) {
		m_Attack = true;

		// まだ充電中でなければ
		if (!m_ChargingAttack) {
			m_ChargeTimer = 0.0f; // タイマーをリセット
			m_ChargingAttack = true; // 充電攻撃フラグを立てる
		}

		// タイマーを更新
		m_ChargeTimer += Time::GetDeltaTime();

		// 一定時間以上スペースキーが押された場合、重撃を発射
		if (m_ChargeTimer >= m_ChargeDuration) {
			// 重撃を実行
			Bullet* bullet = scene->AddGameObject<Bullet>(1);
			bullet->SetPosition(m_Position);
			bullet->SetVelocity(GetForward() * -1.0f); // 充電攻撃の速度を調整

			m_ShotSE->Play(); // 攻撃の音を再生

			// 充電攻撃の変数をリセット
			m_ChargingAttack = false;
			m_ChargeTimer = 0.0f;
		}
	}
	else { // スペースキーが離された場合
		if (m_ChargingAttack && m_ChargeTimer < m_ChargeDuration) {
			// 通常攻撃をトリガー
			Bullet* bullet = scene->AddGameObject<Bullet>(1);
			bullet->SetPosition(m_Position);
			bullet->SetVelocity(GetForward() * -0.5f);

			m_ShotSE->Play(); // 攻撃の音を再生
		}

		// 充電攻撃の変数をリセット
		m_Attack = false;
		m_ChargingAttack = false;
		m_ChargeTimer = 0.0f;
		m_SpaceKeyReleased = true; // スペースキーが離された状態
	}


	//ここからプレイヤーの攻撃ボタンをgame.cppに移動させた※要検証
	//攻撃ボタンの表示
	//scene->AddGameObject<AttackUI>(2);


	//プレイヤーのアニメーション処理
	UpdateAnimation();
}

void Player::Draw()
{
	GameObject::Draw();

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリックス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y + D3DX_PI, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	m_Matrix = world;

	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}

void Player::UpdateAnimation()
{
	m_Model->Update(m_AnimationName.c_str(), m_Time, m_NextAnimationName.c_str(), m_Time,
		m_BlendRate);
	m_Time++;
	m_BlendRate += 0.05f;//ここでアニメーションブレンドの速さを調整
	if (m_BlendRate > 1.0f)
		m_BlendRate = 1.0f;
}

void Player::UpdateGround()
{
	Scene* scene = Manager::GetScene();

	bool move = false;

	//トップビュー
	if (Input::GetKeyPress('W'))
	{
		move = true;
		
		//m_AnimationName = "Run";

		if (Input::GetKeyPress(VK_SHIFT)) // シフトキーが押されている場合はダッシュ
		{
			m_Position.z += 0.1f * m_DashSpeed;

			if (m_NextAnimationName != "Run")
			{
				m_AnimationName = m_NextAnimationName;
				m_NextAnimationName = "Run";
				m_BlendRate = 0.0f;
			}
		}
		else
		{
			m_Position.z += 0.1f;

			if (m_NextAnimationName != "Walk")
			{
				m_AnimationName = m_NextAnimationName;
				m_NextAnimationName = "Walk";
				m_BlendRate = 0.0f;
			}
		}
		m_Rotation.y = D3DX_PI * 1.0f;
	}

	if (Input::GetKeyPress('S'))
	{
		move = true;
		
		//m_AnimationName = "Run";

		if (Input::GetKeyPress(VK_SHIFT)) // シフトキーが押されている場合はダッシュ
		{
			m_Position.z -= 0.1f * m_DashSpeed;

			if (m_NextAnimationName != "Run")
			{
				m_AnimationName = m_NextAnimationName;
				m_NextAnimationName = "Run";
				m_BlendRate = 0.0f;
			}
		}
		else
		{
			m_Position.z -= 0.1f;

			if (m_NextAnimationName != "Walk")
			{
				m_AnimationName = m_NextAnimationName;
				m_NextAnimationName = "Walk";
				m_BlendRate = 0.0f;
			}
		}
		m_Rotation.y = D3DX_PI * 0.0f;
	}

	if (Input::GetKeyPress('A'))
	{
		move = true;
		
		//m_AnimationName = "Run";

		if (Input::GetKeyPress(VK_SHIFT)) // シフトキーが押されている場合はダッシュ
		{
			m_Position.x -= 0.1f * m_DashSpeed;

			if (m_NextAnimationName != "Run")
			{
				m_AnimationName = m_NextAnimationName;
				m_NextAnimationName = "Run";
				m_BlendRate = 0.0f;
			}
		}
		else
		{
			m_Position.x -= 0.1f;

			if (m_NextAnimationName != "Walk")
			{
				m_AnimationName = m_NextAnimationName;
				m_NextAnimationName = "Walk";
				m_BlendRate = 0.0f;
			}
		}
		m_Rotation.y = D3DX_PI * 0.5f;
	}

	if (Input::GetKeyPress('D'))
	{
		move = true;
		
		//m_AnimationName = "Run";

		if (Input::GetKeyPress(VK_SHIFT)) // シフトキーが押されている場合はダッシュ
		{
			m_Position.x += 0.1f * m_DashSpeed;

			if (m_NextAnimationName != "Run")
			{
				m_AnimationName = m_NextAnimationName;
				m_NextAnimationName = "Run";
				m_BlendRate = 0.0f;
			}
		}
		else
		{
			m_Position.x += 0.1f;

			if (m_NextAnimationName != "Walk")
			{
				m_AnimationName = m_NextAnimationName;
				m_NextAnimationName = "Walk";
				m_BlendRate = 0.0f;
			}
		}
		m_Rotation.y = -D3DX_PI * 0.5f;
	}





	//右上方向を向く
	if (Input::GetKeyPress('W') && Input::GetKeyPress('D'))
	{
		m_Rotation.y = -D3DX_PI * 1.0f * 0.75f;
	}

	//左上方向を向く
	if (Input::GetKeyPress('W') && Input::GetKeyPress('A'))
	{
		m_Rotation.y = D3DX_PI * 1.0f * 0.75f;
	}

	//左下方向を向く
	if (Input::GetKeyPress('A') && Input::GetKeyPress('S'))
	{
		m_Rotation.y = D3DX_PI * 0.5f * 0.5f;
	}

	//右下方向を向く
	if (Input::GetKeyPress('S') && Input::GetKeyPress('D'))
	{
		m_Rotation.y = -D3DX_PI * 0.5 * 0.5f;
	}


	if (move == false)
	{
		if (m_NextAnimationName != "Idle")
		{
			m_AnimationName = m_NextAnimationName;
			m_NextAnimationName = "Idle";
			m_BlendRate = 0.0f;
		}
	}

	//ジャンプ
	if (jumpFlag)
	{
		if (Input::GetKeyTrigger('J'))
		{
			//ジャンプのアニメーションを流す
			m_AnimationName = m_NextAnimationName;
			m_NextAnimationName = "Jump";
			m_BlendRate = 0.0f;

			jumpFlag = false;
			m_Time = 0;//ジャンプアニメーションが最初から流れるように初期化する
			m_Velocity.y = 0.35f;
			m_PlayerState = PLAYER_STATE_JUMP;
		}
	}

	//メニューUIの表示
	m_MenuUIFlag = false;

	//メニューを開くとき一回だけでいい処理はここに書く
	if (Input::GetKeyTrigger('M'))
	{
		m_MenuUIFlag = !m_MenuUIFlag; // trueならfalseに、falseならtrueに切り替える

		if (m_MenuUIFlag)
		{
			//アニメーションを流す
			m_AnimationName = m_NextAnimationName;
			m_NextAnimationName = "Idle";
			m_BlendRate = 0.0f;

			m_Time = 0;//アニメーションが最初から流れるように初期化する

			//メニューを開くときの音もここ

			// メニューを表示
			m_MenuUI = scene->AddGameObject<MenuUI>(2);
		}

		m_PlayerState = PLAYER_MENU_UI;
	}
}

void Player::UpdateMenuUI()
{
	Scene* scene = Manager::GetScene();

	if (m_MenuUIFlag && Input::GetKeyTrigger('M'))
	{
		//.アイドル状態に戻す
		m_AnimationName = m_NextAnimationName;
		m_NextAnimationName = "Idle";
		m_BlendRate = 0.0f;

		//メニューを閉じるときの音はここ

		m_MenuUI->SetDestroy();
		m_PlayerState = PLAYER_STATE_GROUND;

	}
}

void Player::UpdateJump()
{
	// ジャンプ中にも移動を可能にする
	float moveSpeed = 0.2f; // 移動速度を調整（必要に応じて変更）

	if (Input::GetKeyPress('W'))
	{
		m_Position.z += moveSpeed;
	}

	if (Input::GetKeyPress('S'))
	{
		m_Position.z -= moveSpeed;
	}

	if (Input::GetKeyPress('A'))
	{
		m_Position.x -= moveSpeed;
	}

	if (Input::GetKeyPress('D'))
	{
		m_Position.x += moveSpeed;
	}



	if (m_IsGround == true)
	{
		//着地したらアイドル状態に戻す
		m_AnimationName = m_NextAnimationName;
		m_NextAnimationName = "Idle";
		m_BlendRate = 0.0f;

		jumpFlag = true;
		m_PlayerState = PLAYER_STATE_GROUND;
	}
}

void Player::TakeDamage()
{
	hp.Add(-200); // 200のダメージ、でも0未満にはならない
		
	if (hp.isEmpty()) {
		// プレイヤーが死亡した場合の処理をここに追加
		// プレイヤーのHPが0以下になったら爆発エフェクトを生成して位置を設定
		Scene* scene = Manager::GetScene();
		scene->AddGameObject<Exprosion>(1)->SetPosition(GetPosition());

		SetDestroy();

		Manager::SetScene<Result>();
		// 他の処理...
	}
}

void Player::SaveGame()
{
	std::ofstream file("saved_game.bin", std::ios::binary);
	if (file.is_open()) {
		// プレイヤーの位置情報を保存する
		file.write(reinterpret_cast<const char*>(&m_Position), sizeof(m_Position));

		// 他のプレイヤーステータス情報を保存する
		file.write(reinterpret_cast<const char*>(&m_PlayerState), sizeof(m_PlayerState));
		file.write(reinterpret_cast<const char*>(&hp), sizeof(hp));
		file.write(reinterpret_cast<const char*>(&m_Velocity), sizeof(m_Velocity));
		file.write(reinterpret_cast<const char*>(&jumpFlag), sizeof(jumpFlag));
		file.write(reinterpret_cast<const char*>(&m_DashSpeed), sizeof(m_DashSpeed));
		file.write(reinterpret_cast<const char*>(&m_Time), sizeof(m_Time));
		file.write(reinterpret_cast<const char*>(&m_BlendRate), sizeof(m_BlendRate));
		file.write(reinterpret_cast<const char*>(&m_IsGround), sizeof(m_IsGround));
		file.write(reinterpret_cast<const char*>(&m_ChargingAttack), sizeof(m_ChargingAttack));
		file.write(reinterpret_cast<const char*>(&m_ChargeTimer), sizeof(m_ChargeTimer));
		file.write(reinterpret_cast<const char*>(&m_SpaceKeyReleased), sizeof(m_SpaceKeyReleased));
		file.write(reinterpret_cast<const char*>(&m_Attack), sizeof(m_Attack));
		file.write(reinterpret_cast<const char*>(&m_MenuUIFlag), sizeof(m_MenuUIFlag));

		file.close();
		std::cout << "Game saved successfully." << std::endl;
	}
	else {
		std::cout << "Failed to save the game." << std::endl;
	}
}
