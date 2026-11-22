#include "stdafx.h"
#include "SE.h"
#include "Scene/Option/SetteiManager.h"

void SE::Register(int no, const char* filePath)
{
	/** SEのファイルパスを登録 */
	g_soundEngine->ResistWaveFileBank(no, filePath);
}

void SE::Play(int no, float volume)
{
	/** 設定画面で決めたSE音量の倍率をかける */
	float finalVolume = volume * SetteiManager::GetInstance()->GetSERate();

	auto* source = NewGO<SoundSource>(0);
	source->Init(no);
	source->SetVolume(finalVolume);
	source->Play(false);
}
