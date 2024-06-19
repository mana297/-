#include "main.h"
#include "renderer.h"
#include "resultlogo.h"
#include "sprite.h"

void Resultlogo::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	//2D�e�N�X�`���̕\��
	//Sprite* sprite = AddCompornent<Sprite>();
	//sprite->Init(100.0f, 100.0f, 300.0f, 200.0f, "asset\\texture\\field001.jpg");

	//�������Ɠ���
	AddCompornent<Sprite>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\result.png");
	//AddCompornent<Sprite>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\kazuha.png");
	//AddCompornent<Sprite>()->Init(400.0f, 100.0f, 300.0f, 200.0f, "asset\\texture\\diluc.png");
	//AddCompornent<Sprite>()->Init(700.0f, 100.0f, 300.0f, 200.0f, "asset\\texture\\diluc.png");

}

void Resultlogo::UnInit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	//���N���X�̃��\�b�h�Ăяo��
	GameObject::UnInit();
}

void Resultlogo::Update()
{
	//���N���X�̃��\�b�h�Ăяo��
	GameObject::Update();
}

void Resultlogo::Draw()
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
