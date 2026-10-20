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
	
	/** SEを登録する */
	m_se->Register((int)SEType::en_ShotSE, "Assets/Sound/SE/ShotSE.wav");
	m_se->Register((int)SEType::en_DecideSE, "Assets/Sound/SE/DecideSE.wav");
	m_se->Register((int)SEType::en_SelectSE, "Assets/Sound/SE/SelectSE.wav");

	/* BGMを登録する
	 * SEtypeの一番後ろから番号を登録する
	 */
	m_bgm->Register((int)SEType::en_NumSE + (int)BGMType::en_TitleBGM,
		"Assets/Sound/BGM/TitleBGM.wav");

	return true;
}

void SoundManager::PlaySE(SEType type, float volume)
{
	/** SEの番号で、登録した音を一回鳴らす */
	m_se->Play((int)type, volume);
}

void SoundManager::PlayBGM(BGMType type, float volume)
{
	/** BGMの番号はSEと番号が被らないようにする */
	m_bgm->Play((int)SEType::en_NumSE + (int)type, volume);
}

void SoundManager::StopBGM()
{
	m_bgm->Stop();
}