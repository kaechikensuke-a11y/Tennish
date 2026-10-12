#pragma once
class SE : public IGameObject
{
public:
	/** 番号とwavファイルを結びつける */
	void Register(int no, const char* filePath);

	/** 一回のみ再生 */
	void Play(int no, float volume);
};

