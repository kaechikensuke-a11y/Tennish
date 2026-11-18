#pragma once
class CharacterAnimator
{
public:
	/** アニメーションの種類 */
	enum EnClip
	{
		en_Idle,      /**     立ち     */
		en_ServeToss, /**     トス     */
		en_ServeHit,  /** サーブを打つ */
		en_ClipNum
	};

	/** アニメーションの読み込み */
	void Load();

	/** アニメーションクリップの配列 */
	AnimationClip* GetAnimationClips() { return m_AnimationClips; }
	int GetAnimationClipNum() const { return en_ClipNum; }

	/** キャラクターモデルの呼出し */
	void Bind(ModelRender* model) { m_model = model; }

	/** ループ再生 */
	void PlayLoop(EnClip clip);

	/** 1回だけ再生 */
	void PlayOneShot(EnClip clip);

	/** ワンショット再生中か */
	bool IsOneShotPlaying() const;

	/** 状態リセット */
	void Reset();

	/** 今再生しているクリップ */
	EnClip GetCurrent() const { return m_current; }

private:
	AnimationClip m_AnimationClips[en_ClipNum];
	ModelRender*  m_model     = nullptr;
	EnClip        m_current   = en_ClipNum;
	bool          m_isOneShot = false;
};

