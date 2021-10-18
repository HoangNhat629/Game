#include "SoundManager.h"

#include "GameConfig.h"

#include <soloud_error.h>

constexpr const char* c_backgroundTrackFile = "..\\Data\\Sounds\\Komiku-School.mp3";
constexpr const char* c_completeTrackFile = "..\\Data\\Sounds\\mixkit-game-level-completed.wav";
constexpr const char* c_failTrackFile = "..\\Data\\Sounds\\mixkit-player-losing.wav";
constexpr const char* c_keyPickupTrackFile = "..\\Data\\Sounds\\mixkit-unlock.wav";

SoundManager::SoundManager()
{
    m_soloudEngine.init();
    SoLoud::result result;

    result = m_backgroundTrack.load(c_backgroundTrackFile);
    if (result != SoLoud::SO_NO_ERROR)
    {
        LOGI("Failed to load sound file %s with error code %d", c_backgroundTrackFile, result);
    }
    
    m_backgroundTrack.setVolume(0.1f);
    m_backgroundTrack.setLooping(true);
    m_backgroundTrack.setSingleInstance(true);

    result = m_completeTrack.load(c_completeTrackFile);
    if (result != SoLoud::SO_NO_ERROR)
    {
        LOGI("Failed to load sound file %s with error code %d", c_completeTrackFile, result);
    }

    m_completeTrack.setVolume(0.12f);

    result = m_failTrack.load(c_failTrackFile);
    if (result != SoLoud::SO_NO_ERROR)
    {
        LOGI("Failed to load sound file %s with error code %d", c_failTrackFile, result);
    }

    m_failTrack.setVolume(0.12f);

    result = m_keyPickupTrack.load(c_keyPickupTrackFile);
    if (result != SoLoud::SO_NO_ERROR)
    {
        LOGI("Failed to load sound file %s with error code %d", c_keyPickupTrackFile, result);
    }

    m_keyPickupTrack.setVolume(0.12f);
}

SoundManager::~SoundManager()
{
    m_soloudEngine.deinit();
}

void SoundManager::ToggleSoundOnOff(bool on)
{
    m_soloudEngine.setGlobalVolume(on ? 1.0f : 0.0f);
}

void SoundManager::PlayBackgroundSound()
{
    m_soloudEngine.play(m_backgroundTrack);
}

void SoundManager::PlayCompleteSound()
{
    m_soloudEngine.play(m_completeTrack);
}

void SoundManager::PlayFailSound()
{
    m_soloudEngine.play(m_failTrack);
}

void SoundManager::PlayKeyPickupSound()
{
    m_soloudEngine.play(m_keyPickupTrack);
}
