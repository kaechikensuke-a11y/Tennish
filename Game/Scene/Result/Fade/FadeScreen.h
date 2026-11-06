#pragma once
class FadeScreen : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/** 画面を白にしていく */
	void StartFadeOut(float seconds);

	/** 白から元の画面に戻す */
	void StartFadeIn(float seconds);

	/** フェードが終わったかどうか */
	bool IsFinished() const { return m_state == State::en_Idle; }
private:
	enum class State
	{
		en_Idle,     /** 動いていない */
		en_FadeOut,  /** 白くなっている最中 */
		en_FadeIn,   /** 白から戻っている最中 */
	};

	SpriteRender m_sprite;
	State m_state = State::en_Idle;
	float m_alpha = 0.0f;      /** 0 = 透明, 1 = 真っ白 */
	float m_timer = 0.0f;
	float m_duration = 1.0f;
};

