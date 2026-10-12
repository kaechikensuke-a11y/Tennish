#include "stdafx.h"
#include "SoundManager.h"
#include "SE/SE.h"
#include "BGM/BGM.h"

SoundManager* SoundManager::m_instance = nullptr;

bool SoundManager::Start()
{
	m_instance = this;

	m_se = new SE;
	m_bgm = new BGM;

	m_se->Register((int)SEType::en_ShotSE, "Assets/Sound/SE/ShotSE.wav");

	return true;
}

void SoundManager::PlaySE(SEType type, float volume)
{
	m_se->Play((int)type, volume);
}

void SoundManager::PlayBGM(BGMType type, float volume)
{
	m_bgm->Play((int)SEType::en_NumSE + (int)type, volume);
}

void SoundManager::StopBGM()
{
	m_bgm->Stop();
}