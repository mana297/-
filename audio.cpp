#include "main.h"
#include "audio.h"
#include "time.h"
#include <cassert>
#include <stdexcept>
#include <vector>

IXAudio2* Audio::m_Xaudio = nullptr;
IXAudio2MasteringVoice* Audio::m_MasteringVoice = nullptr;

void Audio::InitMaster()
{
    // COM初期化
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) throw std::runtime_error("Failed to initialize COM");

    // XAudio2生成
    hr = XAudio2Create(&m_Xaudio, 0);
    if (FAILED(hr)) throw std::runtime_error("Failed to create XAudio2");

    // マスタリングボイス生成
    hr = m_Xaudio->CreateMasteringVoice(&m_MasteringVoice);
    if (FAILED(hr)) throw std::runtime_error("Failed to create mastering voice");
}

void Audio::UninitMaster()
{
    if (m_MasteringVoice) {
        m_MasteringVoice->DestroyVoice();
        m_MasteringVoice = nullptr;
    }
    if (m_Xaudio) {
        m_Xaudio->Release();
        m_Xaudio = nullptr;
    }
    CoUninitialize();
}

bool Audio::LoadWaveFile(const std::string& filename)
{
    HMMIO hmmio = mmioOpen(const_cast<char*>(filename.c_str()), nullptr, MMIO_READ);
    if (!hmmio) return false;

    MMCKINFO riffChunk;
    riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
    if (mmioDescend(hmmio, &riffChunk, nullptr, MMIO_FINDRIFF) != MMSYSERR_NOERROR) {
        mmioClose(hmmio, 0);
        return false;
    }

    MMCKINFO fmtChunk;
    fmtChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if (mmioDescend(hmmio, &fmtChunk, &riffChunk, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
        mmioClose(hmmio, 0);
        return false;
    }

    if (mmioRead(hmmio, reinterpret_cast<HPSTR>(&m_Wfx), sizeof(m_Wfx)) != sizeof(m_Wfx)) {
        mmioClose(hmmio, 0);
        return false;
    }
    mmioAscend(hmmio, &fmtChunk, 0);

    MMCKINFO dataChunk;
    dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if (mmioDescend(hmmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
        mmioClose(hmmio, 0);
        return false;
    }

    m_Length = dataChunk.cksize;
    m_SoundData = std::make_unique<BYTE[]>(m_Length);
    if (mmioRead(hmmio, reinterpret_cast<HPSTR>(m_SoundData.get()), m_Length) != m_Length) {
        mmioClose(hmmio, 0);
        return false;
    }
    m_PlayLength = m_Length / m_Wfx.nBlockAlign;

    mmioClose(hmmio, 0);
    return true;
}

void Audio::Load(const std::string& filename)
{
    if (!LoadWaveFile(filename)) {
        throw std::runtime_error("Failed to load wave file");
    }

    HRESULT hr = m_Xaudio->CreateSourceVoice(&m_SourceVoice, &m_Wfx);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create source voice");
    }
}

void Audio::UnInit()
{
    if (m_SourceVoice) {
        m_SourceVoice->Stop();
        m_SourceVoice->DestroyVoice();
        m_SourceVoice = nullptr;
    }
}

void Audio::Play(bool loop)
{
    if (!m_SourceVoice) return;

    m_SourceVoice->Stop();
    m_SourceVoice->FlushSourceBuffers();

    XAUDIO2_BUFFER buffer = {};
    buffer.AudioBytes = m_Length;
    buffer.pAudioData = m_SoundData.get();
    buffer.Flags = XAUDIO2_END_OF_STREAM;

    if (loop) {
        buffer.LoopBegin = 0;
        buffer.LoopLength = m_PlayLength;
        buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    }

    HRESULT hr = m_SourceVoice->SubmitSourceBuffer(&buffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to submit source buffer");
    }

    m_SourceVoice->SetVolume(m_Volume);  // 現在の音量を設定
    m_SourceVoice->Start(0);
}

void Audio::Stop()
{
    if (m_SourceVoice) {
        m_SourceVoice->Stop();
    }
}

void Audio::SetVolume(float volume)
{
    m_Volume = volume;
    if (m_SourceVoice) {
        m_SourceVoice->SetVolume(volume);
    }
}

void Audio::FadeOut(float duration)
{
    m_FadeOutDuration = duration;
    m_FadeOutTimer = 0.0f;
    m_IsFadingOut = true;
}

void Audio::Update()
{
    if (m_IsFadingOut) {
        m_FadeOutTimer += Time::GetDeltaTime();
        float newVolume = m_Volume * (1.0f - m_FadeOutTimer / m_FadeOutDuration);
        SetVolume(newVolume);

        if (m_FadeOutTimer >= m_FadeOutDuration) {
            m_IsFadingOut = false;
            Stop();
        }
    }
}
