#pragma once
#include "gameobject.h"

class Camera:public GameObject {
private:
    D3DXVECTOR3 m_Target{};
    D3DXMATRIX m_ViewMatrix{};
    D3DXMATRIX m_ProjectionMatrix{};

    float m_DistanceToTarget; // ターゲットとの距離
    float m_Yaw; // Yaw角（水平回転）
    float m_Pitch; // Pitch角（垂直回転）
    float m_MinimumHeight; // カメラのY軸の最低高さ

    D3DXPLANE m_FrustumPlanes[6]; // 視錐体の各平面

    void UpdateFrustumPlanes(); // 視錐体の平面を更新する関数

public:
    void Init();
    void Update();
    void Draw();

    void Zoom(float delta);
    void Rotate(float yawDelta, float pitchDelta);

    D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }

    bool CheckView(D3DXVECTOR3 Position);
};