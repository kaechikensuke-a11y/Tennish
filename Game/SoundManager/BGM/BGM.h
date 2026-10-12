#pragma once
class BGM : public IGameObject
{
public:
	void Register(int no, const char* filePath);
	void Play(int no, float volume);   // すでに鳴っていれば止めてから
	void Stop();

private:
	SoundSource* m_source = nullptr;   // BGM は常に1つだけ持つ
};

