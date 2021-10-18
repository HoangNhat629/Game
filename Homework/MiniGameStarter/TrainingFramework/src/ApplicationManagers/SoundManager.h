#pragma once

#include <soloud.h>
#include <soloud_wav.h>

class SoundManager
{
public:
    SoundManager();
    ~SoundManager();

    void ToggleSoundOnOff(bool on);

    void PlayBackgroundSound();

    void PlayCompleteSound();
    void PlayFailSound();
    void PlayKeyPickupSound();

private:
    SoLoud::Soloud m_soloudEngine;

    SoLoud::Wav m_backgroundTrack;
    SoLoud::Wav m_completeTrack;
    SoLoud::Wav m_failTrack;
    SoLoud::Wav m_keyPickupTrack;
};
