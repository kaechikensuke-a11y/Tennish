#include "stdafx.h"
#include "SE.h"

void SE::Register(int no, const char* filePath)
{
	/** SEのファイルパスを登録 */
	g_soundEngine->ResistWaveFileBank(no, filePath);
}

void SE::Play(int no, float volume)
{
	auto* source = NewGO<SoundSource>(0);
	source->Init(no);
	source->SetVolume(volume);
	source->Play(false);
}
