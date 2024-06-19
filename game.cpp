#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "camera.h"
#include "field.h"
#include "meshField.h"
#include "polygon2D.h"
#include "depthpolygon.h"
#include "polygonwipe.h"
#include "player.h"
#include "child.h"
#include "enemy.h"
#include "bullet.h"
#include "exprosion.h"
#include "cylinder.h"
#include "box.h"
#include "score.h"
#include "result.h"
#include "audio.h"
#include "sky.h"
#include "fade.h"
#include "rock.h"
#include "tree.h"
#include "texUI.h"
#include "attackUI.h"

bool Game::m_LoadFinish = false;

Audio* bgm;

void Game::Load()
{
	Bullet::Load();
	Rock::Load();
	Tree::Load();

	m_LoadFinish = true;
}

void Game::UnLoad()
{
	m_LoadFinish = false;

	Tree::UnLoad();
	Rock::UnLoad();
	Bullet::UnLoad();
}

void Game::Init()
{
	

	/*Camera* camera = new Camera();
		camera->Init();
		//���X�g�\���ɒǉ�
		m_Gameobject.push_back(camera);

		Field* field = new Field();
		field->Init();
		//���X�g�\���ɒǉ�
		m_Gameobject.push_back(field);

		Player* player = new Player();
		player->Init();
		//���X�g�\���ɒǉ�
		m_Gameobject.push_back(player);

		Polygon2D* polygon2D = new Polygon2D();
		polygon2D->Init();
		//���X�g�\���ɒǉ�
		m_Gameobject.push_back(polygon2D);
		*/

	//��̎O�s������ŉ����I
	AddGameObject<Camera>(0);
	AddGameObject<Sky>(1);
	MeshField* meshField = AddGameObject<MeshField>(1);
	

	//AddGameObject<PolygonWipe>(1);

	srand(0);
	
	for (int i = 0; i < 10; i++)
	{
		Rock* rock = AddGameObject<Rock>(1);

		D3DXVECTOR3 pos;
		pos.x = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		pos.z = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		pos.y = 10.0f;/*meshField->GetHight(pos);*/

		rock->SetPosition(pos);
		//rock->SetScale(D3DXVECTOR3(4.0f, 2.0f, 4.0f));
	}

	for (int i = 0; i < 10; i++)
	{
		Tree* tree = AddGameObject<Tree>(1);

		D3DXVECTOR3 pos;
		pos.x = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		pos.z = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		pos.y = meshField->GetHight(pos);

		tree->SetPosition(pos);
		//rock->SetScale(D3DXVECTOR3(4.0f, 2.0f, 4.0f));
	}

	//�~���I�u�W�F�N�g�Ăяo��
	Cylinder* cylinder = AddGameObject<Cylinder>(1);
	cylinder->SetPosition(D3DXVECTOR3(15.0f, 0.0f, 5.0f));
	cylinder->SetScale(D3DXVECTOR3(4.0f, 2.0f, 4.0f));

	//�l�p���I�u�W�F�N�g�Ăяo��
	Box* box = AddGameObject<Box>(1);
	box->SetPosition(D3DXVECTOR3(-15.0f, 0.0f, 5.0f));
	box->SetScale(D3DXVECTOR3(4.0f, 2.0f, 4.0f));

	box->SetPosition(D3DXVECTOR3(0.0f, 2.0f, -15.0f));



	AddGameObject<Enemy>(1);
	//SetPosition�ŔC�ӂ̏ꏊ�Ɏw��
	AddGameObject<Enemy>(1)->SetInitialPosition(D3DXVECTOR3(40.0f, 2.0f, -4.0f));
	//AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(40.0f, 2.0f, -1.0f));
	//AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(40.0f, 2.0f, 2.0f));
	//AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(40.0f, 2.0f, 5.0f));
	//AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(40.0f, 2.0f, 8.0f));

	//�}�g���b�N�X���g���ăv���C���[�Ɠ����ʒu�ɉ����e�q�t���������Ƃ�
	Player* player = AddGameObject<Player>(1);
	player->SetPosition(D3DXVECTOR3(-1.0f, 0.0f, -4.0f));
	//AddGameObject<Child>(1)->SetParent(player);

	AddGameObject<Score>(2);
	AddGameObject<Fade>(2);

	
	bgm = AddGameObject<GameObject>(0)->AddCompornent<Audio>();

	bgm->Load("asset\\audio\\BGM\\harpohikunezumi.wav");
	bgm->Play(true);

	// �Z�[�u�f�[�^�����[�h����
	player->LoadGame(); // �Z�[�u�f�[�^�����[�h����
	
}

void Game::UnInit()
{
	Scene::UnInit();

	Game::UnLoad();
}

void Game::Update()
{
	Scene::Update();

	// ���ʒ���
	if (Input::GetKeyTrigger(VK_OEM_COMMA)) // '<' �L�[
	{
		float volume = bgm->GetVolume();
		volume = std::max(0.0f, volume - 0.1f); // ���ʂ������� (�ŏ��l��0.0)
		bgm->SetVolume(volume);
	}
	if (Input::GetKeyTrigger(VK_OEM_PERIOD)) // '>' �L�[
	{
		float volume = bgm->GetVolume();
		volume = std::min(1.0f, volume + 0.1f); // ���ʂ��グ�� (�ő�l��1.0)
		bgm->SetVolume(volume);
	}

}
