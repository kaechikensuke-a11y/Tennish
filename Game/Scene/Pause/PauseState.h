#pragma once
class PauseState
{
public:
	/** 動かしてはいけない間は true */
	static bool IsPaused();

	/** ポーズ開始 */
	static void Pause();

	/** ポーズ終了。数フレームだけ待ってから動き出す */
	static void Resume();

	/** 完全にリセットする(Game が消えるときに使う) */
	static void Reset();

	/** 毎フレーム1回、Game::Update から呼ぶ */
	static void Tick();

private:
	/** 再開してから動き出すまで待つフレーム数 */
	enum 
	{
		RESUME_WAIT_FRAMES = 2 
	};

	static bool m_paused;
	static int  m_resumeFrames;
};

