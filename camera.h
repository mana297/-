#pragma once
#include "gameobject.h"

class Camera:public GameObject {
private:
    D3DXVECTOR3 m_Target{};
    D3DXMATRIX m_ViewMatrix{};
    D3DXMATRIX m_ProjectionMatrix{};

    float m_DistanceToTarget; // ^[QbgΖΜ£
    float m_Yaw; // Yawpi½ρ]j
    float m_Pitch; // PitchpiΌρ]j
    float m_MinimumHeight; // JΜY²ΜΕα³

    D3DXPLANE m_FrustumPlanes[6]; // ΜΜe½Κ

    void UpdateFrustumPlanes(); // ΜΜ½ΚπXV·ιΦ

public:
    void Init();
    void Update();
    void Draw();

    void Zoom(float delta);
    void Rotate(float yawDelta, float pitchDelta);

    D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }

    bool CheckView(D3DXVECTOR3 Position);
};