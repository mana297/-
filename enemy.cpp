#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include <ctime>
#include "enemy.h"
#include "player.h"
#include "shadow.h"
#include "exprosion.h"
#include "cylinder.h"
#include "box.h"
#include "rock.h"
#include "meshField.h"
#include "Time.h"
#include "audio.h"

void Enemy::Init()
{
	//アニメーションの読み込み
	m_Model = new AnimationModel();
	m_Model->Load("asset\\enemy\\E_Vampire.fbx");
	m_Model->LoadAnimation("asset\\model\\Bot_Run.fbx", "Run");
	m_Model->LoadAnimation("asset\\model\\Walking.fbx", "Walk");
	m_Model->LoadAnimation("asset\\enemy\\Idle.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\Jumping.fbx", "Jump");

	//最初はアイドル状態にする
	m_AnimationName = "Idle";
	m_NextAnimationName = "Idle";

	m_Scale = D3DXVECTOR3(0.018f, 0.018f, 0.018f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

	m_LookPlayer = AddCompornent<Audio>();
	m_LookPlayer->Load("asset\\audio\\ENEMY\\02_monster_before.wav");

	//攻撃関係のボイス
	m_AttackPlayer1 = AddCompornent<Audio>();
	m_AttackPlayer1->Load("asset\\audio\\ENEMY\\07_monster_attack.wav");
	m_AttackPlayer2 = AddCompornent<Audio>();
	m_AttackPlayer2->Load("asset\\audio\\ENEMY\\09_monster_attack.wav");
	m_AttackPlayer3 = AddCompornent<Audio>();
	m_AttackPlayer3->Load("asset\\audio\\ENEMY\\11_monster_attack.wav");

	m_Shadow = AddCompornent<Shadow>();

	// 初期化時にランダムシードを設定
	srand(static_cast<unsigned>(time(NULL)));
}

void Enemy::UnInit()
{
	GameObject::UnInit();
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


}

void Enemy::Update()
{
	GameObject::Update();

	Scene* scene = Manager::GetScene();

	// プレイヤーの位置情報を取得
	D3DXVECTOR3 playerPosition = Manager::GetScene()->GetGameObject<Player>()->GetPosition();

	// エネミーの状態に応じて処理を分岐
	switch (m_EnemyState)
	{
	case STATE_STAND:
		// STATE_STAND の場合、アイドル状態処理を行う
		EnemyStand();
		break;
	case STATE_NORMAL:
		// STATE_NORMAL の場合、初期位置に戻る処理を行う
		ReturnToPosition();
		break;
	case STATE_CHASE_PLAYER:
		// STATE_CHASE_PLAYER の場合、プレイヤーを追跡する処理を行う
		ChasePlayer();
		break;
	case STATE_ATTACK:
		// STATE_ATTACK の場合、プレイヤーを攻撃する処理を行う
		AttackPlayer();
		break;
	default:
		break;
		// 他のエネミーの状態に関する処理...
	}


	float groundHeight = 0.0f;  // デフォルトの地面高さを設定

	// MeshFieldから現在位置の地面高さを取得するコードを追加
	MeshField* meshField = Manager::GetScene()->GetGameObject<MeshField>();
	groundHeight = meshField->GetHight(m_Position);

	// エネミーの位置を地面の高さに制限
	if (m_Position.y < groundHeight)
	{
		m_Position.y = groundHeight;
	}




	// プレイヤーとエネミーの距離を計算
	D3DXVECTOR3 distanceVector = playerPosition - m_Position;
	float distanceToPlayer = D3DXVec3Length(&distanceVector);

	// 一定の距離以下にプレイヤーがいるか判定
	if (distanceToPlayer < 10.0f) {//エネミーがどのくらいの距離になったら追跡するかの距離
		m_EnemyState = STATE_CHASE_PLAYER;

		// エネミーとプレイヤーの距離が5未満の場合、攻撃処理に移行
		if (distanceToPlayer < 10.0f) {
			// ここに攻撃処理を実行するコードを追加
			// クールダウン時間を進める
			m_TimeSinceLastAttack += Time::GetDeltaTime(); // Time::GetDeltaTime() は経過時間を返す関数

			if (m_TimeSinceLastAttack >= m_AttackCooldown) {
				// クールダウンが終了したら攻撃を実行
				/*
				多分ここをアタックプライヤーではなくバレットとか呼び出すと
				玉発射などできるようになる
				それでその玉発射の中にプレイヤーに当たったらアタックプレイヤーを実行すればいい感じになると思う
				*/
				AttackPlayer();
				m_TimeSinceLastAttack = 0.0f; // 攻撃後、クールダウンのリセット
			}
		}
	}
	else if (m_EnemyState != STATE_CHASE_PLAYER) {
		// 一定の距離以上にプレイヤーがいる場合、エネミーの状態を通常モードに戻す
		m_EnemyState = STATE_NORMAL;
	}
	else
	{
		m_EnemyState = STATE_STAND;
	}



	//エネミーの直下に影を表示
	D3DXVECTOR3 shadowPosition = m_Position;
	shadowPosition.y = groundHeight + 0.01f;
	m_Shadow->SetPosition(shadowPosition);

	//エネミーのアニメーション処理
	UpdateAnimation();
}

void Enemy::Draw()
{
	GameObject::Draw();

	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	//四錐形カリング（カメラ外にオブジェクトがある時表示しない）
	if (!camera->CheckView(m_Position))
	{
		return;
	}

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

void Enemy::UpdateAnimation()
{
	m_Model->Update(m_AnimationName.c_str(), m_Time, m_NextAnimationName.c_str(), m_Time,
		m_BlendRate);
	m_Time++;
	m_BlendRate += 0.05f;//ここでアニメーションブレンドの速さを調整
	if (m_BlendRate > 1.0f)
		m_BlendRate = 1.0f;
}

void Enemy::DecreaseHP(int damage)
{
	// 弾の威力分だけ HP を減らす
	m_HP -= damage; //Bulletで威力書いてるよ

	// HPが0以下になったらエネミーを破壊する
	if (m_HP <= 0)
	{
		// エネミーのHPが0以下になったら爆発エフェクトを生成して位置を設定
		Scene* scene = Manager::GetScene();
		scene->AddGameObject<Exprosion>(1)->SetPosition(GetPosition());

		SetDestroy();
		// 他の処理...
	}
}

void Enemy::ReturnToPosition()
{
	// 現在のアニメーションが「Walk」でない場合は、「Walk」に設定
/*	if (m_AnimationName != "Walk") {
		m_AnimationName = "Walk";
		m_NextAnimationName = "Walk";
		m_BlendRate = 0.0f;
	}*/

	// エネミーの現在位置から初期位置までの距離を計算
	D3DXVECTOR3 distanceVector = m_InitialPosition - m_Position;
	float distanceToInitialPosition = D3DXVec3Length(&distanceVector);

	// 一定の移動速度で初期位置に向かって移動
	float moveSpeed = 0.1f; // 移動速度を調整

	if (distanceToInitialPosition > 0.1f) // ある程度の距離がある場合のみ移動
	{
		// 移動ベクトルを計算
		D3DXVECTOR3 moveDirection = distanceVector / distanceToInitialPosition;

		// エネミーを移動
		m_Position += moveDirection * moveSpeed;
	}
}

void Enemy::ChasePlayer()
{
	// 現在のアニメーションが「Walk」でない場合は、「Walk」に設定
	if (m_AnimationName != "Walk") {
		m_AnimationName = "Walk";
		m_NextAnimationName = "Walk";
		m_BlendRate = 0.0f;
	}
	// 一定の距離以下にプレイヤーがいる場合、エネミーの状態を追跡モードに変更
	//m_LookPlayer->Play(); // プレイヤーを見つけた時の音を再生

	// プレイヤーの位置情報を取得
	D3DXVECTOR3 playerPosition = Manager::GetScene()->GetGameObject<Player>()->GetPosition();

	// プレイヤーへの方向ベクトルを計算
	D3DXVECTOR3 directionToPlayer = playerPosition - m_Position;
	D3DXVec3Normalize(&directionToPlayer, &directionToPlayer);

	// エネミーの向きを回転行列から取得
	D3DXVECTOR3 enemyForward = directionToPlayer; // プレイヤーの方向ベクトルと同じ

	// エネミーの位置を更新
	m_Position += enemyForward * 0.05f; // エネミーの速度
}

void Enemy::EnemyStand()
{
	// 現在のアニメーションが「Idle」でない場合は、「Idle」に設定
	if (m_AnimationName != "Idle") {
		m_AnimationName = "Idle";
		m_NextAnimationName = "Idle";
		m_BlendRate = 0.0f;
	}
}

// エネミーがプレイヤーを攻撃する関数
void Enemy::AttackPlayer() {
	// プレイヤーのクラスにアクセスするためのポインタを取得
	Player* player = Manager::GetScene()->GetGameObject<Player>();

	// プレイヤーの位置を取得
	D3DXVECTOR3 playerPosition = player->GetPosition();

	// プレイヤーにダメージを与える
	player->TakeDamage();

	//攻撃時のボイス
	int randomVoice = rand() % 3; // Init関数に用意した攻撃ボイスの数
	if (randomVoice == 0) {
		m_AttackPlayer1->Play(); // ボイス1を再生
	}
	else if (randomVoice == 1) {
		m_AttackPlayer2->Play(); // ボイス2を再生
	}
	else {
		m_AttackPlayer3->Play(); // ボイス3を再生
	}

	// ここで攻撃エフェクトをプレイヤーの位置に表示する
	Scene* scene = Manager::GetScene();
	scene->AddGameObject<Exprosion>(1)->SetPosition(playerPosition);
}
