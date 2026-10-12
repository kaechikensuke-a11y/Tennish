#include "stdafx.h"
#include "BGM.h"

void BGM::Register(int no, const char* filePath)
{
	g_soundEngine->ResistWaveFileBank(no, filePath);
}

void BGM::Play(int no, float volume)
{
	Stop();   // 二重に鳴らさない

	m_source = NewGO<SoundSource>(0);
	m_source->Init(no);
	m_source->SetVolume(volume);
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
