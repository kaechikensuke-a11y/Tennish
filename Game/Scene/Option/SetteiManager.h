#pragma once
class SetteiManager : public IGameObject
{
public:
	static SetteiManager* GetInstance() { return m_instance; }

	bool Start();

	/** 音量の段階 */
	static constexpr int VOLUME_MAX = 10;

	/** BGMの音量 */
	int GetBGMLevel() const { return m_bgmLevel; }
	void SetBGMLevel(int level);

	/** SE音量 */
	int GetSELevel() const { return m_seLevel; }
	void SetSELevel(int level);

	/** 実際にかける倍率 */
	float GetBGMRate() const { return m_bgmLevel / (float)VOLUME_MAX; }
	float GetSERate() const { return m_seLevel   / (float)VOLUME_MAX; }

	/** バイブレーション */
	bool IsVibration() const { return m_vibration; }
	void SetVibration(bool on) { m_vibration = on; }

private:
	static SetteiManager* m_instance;

	/** 初期値 */
	int m_bgmLevel   = 5;
	int m_seLevel    = 5;
	bool m_vibration = true;
};

