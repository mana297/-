#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "scene.h"
#include "input.h"

void Camera::Init()
{
    m_DistanceToTarget = 10.0f; // �^�[�Q�b�g����̏�������
    m_Yaw = 0.0f;
    m_Pitch = 0.0f;
    m_MinimumHeight = 5.0f; // �J������Y���̍Œፂ����ݒ�

    m_Position = D3DXVECTOR3(0.0f, 5.0f, -m_DistanceToTarget);
    m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void Camera::Update() 
{
    Scene* scene = Manager::GetScene();
    Player* player = scene->GetGameObject<Player>();

    m_Target = player->GetPosition();

    // Yaw��Pitch���g���ăJ�����̈ʒu���X�V
    D3DXVECTOR3 offset;
    offset.x = m_DistanceToTarget * cosf(m_Pitch) * cosf(m_Yaw);
    offset.y = m_DistanceToTarget * sinf(m_Pitch);
    offset.z = m_DistanceToTarget * cosf(m_Pitch) * sinf(m_Yaw);

    m_Position = m_Target + offset;

    // �J������Y���̍������Œፂ���ɒ���
    if (m_Position.y < m_MinimumHeight) {
        m_Position.y = m_MinimumHeight;
    }

    // �r���[�}�g���b�N�X�ݒ�
    D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &up);

    Renderer::SetViewMatrix(&m_ViewMatrix);

    // �v���W�F�N�V�����}�g���N�X�ݒ�
    D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
    Renderer::SetProjectionMatrix(&m_ProjectionMatrix);

    // �����̂̕��ʂ��X�V
    UpdateFrustumPlanes();
   
    // ���j���[UI���A�N�e�B�u�łȂ��ꍇ�ɃJ�����̑����L���ɂ���
    if (!player->MenuUIActive()) {
        // �J�����̃Y�[���C��/�Y�[���A�E�g
        if (Input::GetKeyPress(VK_UP)) {
            Zoom(-0.5f); // �Y�[���C��
        }
        if (Input::GetKeyPress(VK_DOWN)) {
            Zoom(0.5f); // �Y�[���A�E�g
        }

        // �J�����̉�]
        if (Input::GetKeyPress(VK_LEFT)) {
            Rotate(-0.02f, 0.0f); // ���ɉ�]
        }
        if (Input::GetKeyPress(VK_RIGHT)) {
            Rotate(0.02f, 0.0f); // �E�ɉ�]
        }
        // if (Input::GetKeyPress('W')) {
        //     Rotate(0.0f, 0.02f); // ��ɉ�]
        // }
        // if (Input::GetKeyPress('S')) {
        //     Rotate(0.0f, -0.02f); // ���ɉ�]
        // }
    }
}

void Camera::Draw() 
{
    Renderer::SetViewMatrix(&m_ViewMatrix);
    Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

void Camera::Zoom(float delta) 
{
    m_DistanceToTarget += delta;
    if (m_DistanceToTarget < 1.0f) {
        m_DistanceToTarget = 1.0f;
    }
    else if (m_DistanceToTarget > 100.0f) {
        m_DistanceToTarget = 100.0f;
    }
}

void Camera::Rotate(float yawDelta, float pitchDelta)
{
    m_Yaw += yawDelta;
    m_Pitch += pitchDelta;

    // �s�b�`�p�̐���
    if (m_Pitch > D3DX_PI / 2.0f) {
        m_Pitch = D3DX_PI / 2.0f;
    }
    else if (m_Pitch < -D3DX_PI / 2.0f) {
        m_Pitch = -D3DX_PI / 2.0f;
    }
}

void Camera::UpdateFrustumPlanes() 
{
    D3DXMATRIX viewProj = m_ViewMatrix * m_ProjectionMatrix;

    // ������
    m_FrustumPlanes[0].a = viewProj._14 + viewProj._11;
    m_FrustumPlanes[0].b = viewProj._24 + viewProj._21;
    m_FrustumPlanes[0].c = viewProj._34 + viewProj._31;
    m_FrustumPlanes[0].d = viewProj._44 + viewProj._41;

    // �E����
    m_FrustumPlanes[1].a = viewProj._14 - viewProj._11;
    m_FrustumPlanes[1].b = viewProj._24 - viewProj._21;
    m_FrustumPlanes[1].c = viewProj._34 - viewProj._31;
    m_FrustumPlanes[1].d = viewProj._44 - viewProj._41;

    // �㕽��
    m_FrustumPlanes[2].a = viewProj._14 - viewProj._12;
    m_FrustumPlanes[2].b = viewProj._24 - viewProj._22;
    m_FrustumPlanes[2].c = viewProj._34 - viewProj._32;
    m_FrustumPlanes[2].d = viewProj._44 - viewProj._42;

    // ������
    m_FrustumPlanes[3].a = viewProj._14 + viewProj._12;
    m_FrustumPlanes[3].b = viewProj._24 + viewProj._22;
    m_FrustumPlanes[3].c = viewProj._34 + viewProj._32;
    m_FrustumPlanes[3].d = viewProj._44 + viewProj._42;

    // �ߕ���
    m_FrustumPlanes[4].a = viewProj._13;
    m_FrustumPlanes[4].b = viewProj._23;
    m_FrustumPlanes[4].c = viewProj._33;
    m_FrustumPlanes[4].d = viewProj._43;

    // ������
    m_FrustumPlanes[5].a = viewProj._14 - viewProj._13;
    m_FrustumPlanes[5].b = viewProj._24 - viewProj._23;
    m_FrustumPlanes[5].c = viewProj._34 - viewProj._33;
    m_FrustumPlanes[5].d = viewProj._44 - viewProj._43;

    // ���ʂ𐳋K��
    for (int i = 0; i < 6; i++) {
        D3DXPlaneNormalize(&m_FrustumPlanes[i], &m_FrustumPlanes[i]);
    }
}

bool Camera::CheckView(D3DXVECTOR3 Position) 
{
    for (int i = 0; i < 6; i++) {
        if (D3DXPlaneDotCoord(&m_FrustumPlanes[i], &Position) < 0.0f) {
            return false;
        }
    }
    return true;
}
