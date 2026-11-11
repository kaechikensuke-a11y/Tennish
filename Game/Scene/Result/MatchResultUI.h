#pragma once
class MatchResultUI : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/** 終了画像の出現を始める */
	void ShowEndImage();

	/** 勝利演出UI */
	void ShowResult(int playerGames, int enemyGames);

	/** 終了画像が出て、消えるところまで終わったか */
	bool IsEndImageFinished() const { return m_phase == Phase::en_Done; }

private:
	enum class Phase
	{
		en_Hidden,    /** まだ描画していない */
		en_Appear,	  /** アニメーションで出現 */
		en_Hold,	  /** 表示したまま保持する */
		en_Disappear, /** 出現の逆再生で消える */
		en_Done		  /** 終わり */
	};

	SpriteRender m_endImage;
	SpriteRender m_endBackImage;
	SpriteRender m_band;
	SpriteRender m_winText;
	SpriteRender m_flow;
	Phase m_phase = Phase::en_Hidden;
	float m_timer = 0.0f;
	float m_scale = 0.0f;
	float m_resultTimer = 0.0f;
	float m_scoreScale = 0.0f;

	bool m_resultActive = false;
};

