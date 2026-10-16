#pragma once
class Ball : public IGameObject
{
public:
	Ball() {}
	~Ball(){}

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	
	/** ボールを持っている間は物理計算を止めて、手の位置に固定する。 */
	void SetHeld(bool held) { m_isHeld = held; }
	void SetPosition(const Vector3& pos) { m_ballPosition = pos; }
	void SetVelocity(const Vector3& vel) { m_ballSpeed    = vel; }
	const Vector3& GetPosition() const { return m_ballPosition; }

	/** ボールの速度(進行方向の判定や落下地点の予測に使う) */
	const Vector3& GetVelocity() const { return m_ballSpeed; }

	/** 今の位置と速度から、地面に着地する地点を予測して返す */
	Vector3 PredictLanding() const;

	/** 今の位置と速度から */

	/** ボールのバウンド等関数 */
	int GetBounceCount() const { return m_bounceCount; }
	void ResetBounceCount() { m_bounceCount = 0; }

	void SetShotEffect(float curveAccelX, float gravityScale, float bounceScale)
	{
		m_curveAccelX = curveAccelX;
		m_gravityScale = gravityScale;
		m_bounceScale = bounceScale;
	}

private:
	/** ボールのモデル */
	ModelRender m_ballRender;

	/** ボールのポジション */
	Vector3 m_ballPosition = Vector3::Zero;
	
	/** ボールの速度 */
	Vector3 m_ballSpeed = Vector3::Zero;

	/** ボールを持っているかどうか */
	bool m_isHeld = true;

	float m_curveAccelX = 0.0f;
	float m_gravityScale = 1.0f;
	float m_bounceScale = 1.0f;

	/** ボールのバウンド回数 */
	uint8_t m_bounceCount = 0;
};

