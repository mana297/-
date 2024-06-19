#pragma once
#include "gameobject.h"

class Camera:public GameObject {
private:
    D3DXVECTOR3 m_Target{};
    D3DXMATRIX m_ViewMatrix{};
    D3DXMATRIX m_ProjectionMatrix{};

    float m_DistanceToTarget; // �^�[�Q�b�g�Ƃ̋���
    float m_Yaw; // Yaw�p�i������]�j
    float m_Pitch; // Pitch�p�i������]�j
    float m_MinimumHeight; // �J������Y���̍Œፂ��

    D3DXPLANE m_FrustumPlanes[6]; // �����̂̊e����

    void UpdateFrustumPlanes(); // �����̂̕��ʂ��X�V����֐�

public:
    void Init();
    void Update();
    void Draw();

    void Zoom(float delta);
    void Rotate(float yawDelta, float pitchDelta);

    D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }

    bool CheckView(D3DXVECTOR3 Position);
};