#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "scene.h"
#include "texUI.h"
#include "player.h"
#include "input.h"
#include "sprite.h"
#include "menuUI.h"
#include "meshField.h"
#include "audio.h"

void MenuUI::Init()
{
	// ����������
	m_SelectedObject = MenuObject::Item;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	                                               //�I�[
	AddCompornent<Sprite>()->Init(100.0f , 100.0f, 1080.0f, 50.0f, "asset\\texture\\back.png");
	AddCompornent<Sprite>()->Init(100.0f, 100.0f, 1080.0f, 50.0f, "asset\\texture\\flame.png");
	
	//�܂�����ɐݒ肵�Ă���
	Sprite* backTex = AddCompornent<Sprite>();
	backTex->Init(100.0f, 200.0f, 400.0f, 300.0f, "asset\\texture\\back.png");
	backTex->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	AddCompornent<Sprite>()->Init(100.0f, 200.0f, 400.0f, 300.0f, "asset\\texture\\comingsoon.png");

	AddCompornent<TexUI>()->Init();


	//m_DropItemSE = AddCompornent<Audio>();
	//m_DropItemSE->Load("asset\\audio\\SE\\itemdrop.wav");

	//m_StandItemSE = AddCompornent<Audio>();
	//m_StandItemSE->Load("asset\\audio\\SE\\itemdrop.wav");
}

void MenuUI::UnInit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	//���N���X�̃��\�b�h�Ăяo��
	GameObject::UnInit();
}

void MenuUI::Update()
{
	//���N���X�̃��\�b�h�Ăяo��
	GameObject::Update();

	//// Player�N���X����t���O�̏�Ԃ��擾
	Player* player = Manager::GetScene()->GetGameObject<Player>();
	bool menuflag = player->GetMenuUIFlag();


	// �o�b�O�t���O��True�̏ꍇ�̓e�[�u���I�u�W�F�N�g�𐶐�����
	if (menuflag)
	{

		// �I�𒆂̃I�u�W�F�N�g���X�V
		UpdateSelectedObject();

		// �I�𒆂̃I�u�W�F�N�g�ɉ����ď������s��
		switch (m_SelectedObject) {
		case MenuObject::Item:
		{
			
		}
		break;
		case MenuObject::Quest:
		{
		
		}
		break;
		case MenuObject::Information:
		{
			
		}
		break;
		case MenuObject::System:
		{

		}
		break;
		default:
			// �����I������Ă��Ȃ��ꍇ�̏���
			break;
		}
	}
}


void MenuUI::Draw()
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���b�N�X�ݒ�
	Renderer::SetWorldViewProjection2D();


	//���N���X�̃��\�b�h�Ăяo��
	GameObject::Draw();

}





void MenuUI::UpdateSelectedObject()
{
	if (Input::GetKeyTrigger(VK_RIGHT)) {
		// �E���L�[�������ꂽ�ꍇ�A�I�𒆂̃I�u�W�F�N�g�����ɕύX
		switch (m_SelectedObject) {
		case MenuObject::Item:
			m_SelectedObject = MenuObject::Quest;
			break;
		case MenuObject::Quest:
			m_SelectedObject = MenuObject::Information;
			break;
		case MenuObject::Information:
			// �֎q���I������Ă���ꍇ�͍Ăэŏ��̃I�u�W�F�N�g�ɖ߂�
			m_SelectedObject = MenuObject::System;
			break;
		case MenuObject::System:
			// �֎q���I������Ă���ꍇ�͍Ăэŏ��̃I�u�W�F�N�g�ɖ߂�
			m_SelectedObject = MenuObject::System;
			break;
		default:
			break;
		}
	}
	else if (Input::GetKeyTrigger(VK_LEFT)) {
		// �����L�[�������ꂽ�ꍇ�A�I�𒆂̃I�u�W�F�N�g��O�ɕύX
		switch (m_SelectedObject) {
		case MenuObject::Item:
			// �ŏ��̃I�u�W�F�N�g�̏ꍇ�͍Ō�̃I�u�W�F�N�g�Ɉړ�
			m_SelectedObject = MenuObject::Item;
			break;
		case MenuObject::Quest:
			m_SelectedObject = MenuObject::Item;
			break;
		case MenuObject::Information:
			m_SelectedObject = MenuObject::Quest;
			break;
		case MenuObject::System:
			m_SelectedObject = MenuObject::Information;
			break;
		default:
			break;
		}
	}
}

