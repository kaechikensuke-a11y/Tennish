#pragma once

class SE;
class BGM;

enum class SEType
{
	en_ShotSE,
	en_NumSE
};

enum class BGMType
{
	en_NumBGM
};

class SoundManager : public IGameObject
{
public:
	static SoundManager* GetInstance() { return m_instance; }

	bool Start();

	// SE を1回鳴らす
	void PlaySE(SEType type, float volume = 1.0f);

	// BGM を再生 / 停止
	void PlayBGM(BGMType type, float volume = 0.5f);
	void StopBGM();

private:
	static SoundManager* m_instance;

	SE* m_se = nullptr;
	BGM* m_bgm = nullptr;
};

