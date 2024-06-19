#pragma once

#include <xaudio2.h>
#include <memory>
#include <string>
#include "gameobject.h"

class Audio : public Compornent
{
private:
    static IXAudio2* m_Xaudio;
    static IXAudio2MasteringVoice* m_MasteringVoice;

    IXAudio2SourceVoice* m_SourceVoice{};
    std::unique_ptr<BYTE[]> m_SoundData;
    WAVEFORMATEX m_Wfx{};
    int m_Length{};
    int m_PlayLength{};
    float m_Volume = 1.0f;  // åªç›ÇÃâπó 

    bool LoadWaveFile(const std::string& filename);

    bool m_IsFadingOut = false;
    float m_FadeOutDuration = 0.0f;
    float m_FadeOutTimer = 0.0f;
public:
    static void InitMaster();
    static void UninitMaster();

    using Compornent::Compornent;

    void UnInit();

    void Load(const std::string& filename);
    void Play(bool loop = false);
    void Stop();
    void SetVolume(float volume);
    float GetVolume() const { return m_Volume; }  // âπó ÇéÊìæÇ∑ÇÈÉÅÉ\ÉbÉh

    void FadeOut(float duration);

    void Update();
};
