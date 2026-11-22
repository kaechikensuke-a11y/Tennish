#pragma once
class BGM : public IGameObject
{
public:
	void Register(int no, const char* filePath);
	void Play(int no, float volume);   // すでに鳴っていれば止めてから鳴らす
	void Stop();
	void ApplyVolume();

private:
	SoundSource* m_source = nullptr;   // BGM は常に1つだけ持つ

	/** 再生時に指定された */
	float m_baseVolume = 1.0f;
};

