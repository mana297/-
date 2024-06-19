#include "main.h"
#include "renderer.h"
#include "titlelogo.h"
#include "sprite.h"
#include "time.h"

void Titlelogo::Init() {
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\unlitTextureVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\unlitTexturePS.cso");

    // 2D�e�N�X�`���̕\��
    AddCompornent<Sprite>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\back.png");
    AddCompornent<Sprite>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\titlelogo.png");


    m_Titlelogo = AddCompornent<Sprite>();
    m_Titlelogo->Init(390.0f, 300.0f, 500.0f, 300.0f, "asset\\texture\\press_enter_key.png");
    m_Titlelogo->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

    m_Timer = 0.0f;
    m_FadeOut = true;
}

void Titlelogo::UnInit() {
    m_VertexLayout->Release();
    m_VertexShader->Release();
    m_PixelShader->Release();

    // ���N���X�̃��\�b�h�Ăяo��
    GameObject::UnInit();
}

void Titlelogo::Update() 
{
    // ���N���X�̃��\�b�h�Ăяo��
    GameObject::Update();

    m_Timer += Time::GetDeltaTime();

    float alpha = m_Titlelogo->GetColor().a;
    if (m_FadeOut) {
        alpha -= Time::GetDeltaTime() / 2.0f; // 2�b�ԂŃ��l��0�ɂ���
        if (alpha <= 0.0f) {
            alpha = 0.0f;
            m_FadeOut = false; // �t�F�[�h�C���ɐ؂�ւ���
            m_Timer = 0.0f;
        }
    }
    else {
        alpha += Time::GetDeltaTime() / 2.0f; // 2�b�ԂŃ��l��1�ɂ���
        if (alpha >= 1.0f) {
            alpha = 1.0f;
            m_FadeOut = true; // �t�F�[�h�A�E�g�ɐ؂�ւ���
            m_Timer = 0.0f;
        }
    }

    m_Titlelogo->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, alpha));
}

void Titlelogo::Draw()
{
    // ���̓��C�A�E�g�ݒ�
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    // �V�F�[�_�ݒ�
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    // �}�g���b�N�X�ݒ�
    Renderer::SetWorldViewProjection2D();

    // ���N���X�̃��\�b�h�Ăяo��
    GameObject::Draw();
}
