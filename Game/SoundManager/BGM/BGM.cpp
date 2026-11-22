#include "stdafx.h"
#include "BGM.h"
#include "Scene/Option/SetteiManager.h"

void BGM::Register(int no, const char* filePath)
{
	g_soundEngine->ResistWaveFileBank(no, filePath);
}

void BGM::Play(int no, float volume)
{
	Stop();   // 二重に鳴らさない

	/** 基本音量を記憶させる */
	m_source = NewGO<SoundSource>(0);
	m_source->Init(no);

	m_source->SetVolume(m_baseVolume * SetteiManager::GetInstance()->GetBGMRate());
	m_source->Play(true);   // true: ループ
}

void BGM::Stop()
{
	if (m_source != nullptr)
	{
		DeleteGO(m_source);
		m_source = nullptr;
	}
}

void BGM::ApplyVolume()
{
	/** 再生していなければ何もしない */
	if (m_source == nullptr)
	{
		return;
	}

	/** 覚えておいた基本音量に、最新の設定倍率をかけ直す */
	m_source->SetVolume(m_baseVolume * SetteiManager::GetInstance()->GetBGMRate());
}
