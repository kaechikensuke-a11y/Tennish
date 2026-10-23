#pragma once
class Character;
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
	void ResetBounceCount()
	{ 
		m_bounceCount = 0;
		m_isHasBounced = false;
		m_firstBouncePos = Vector3::Zero;
		m_isHitNet = false;
		m_isCrossedNet = false;
	}

	void SetShotEffect(float curveAccelX, float gravityScale, float bounceScale)
	{
		m_curveAccelX = curveAccelX;
		m_gravityScale = gravityScale;
		m_bounceScale = bounceScale;
	}

	/** 最後に打った人 */
	void SetLastHitPlayer(const Character* hit) { m_lastHitPlayer = hit; }
	const Character* GetLastHitPlayer() const { return m_lastHitPlayer; }

	/** 1バウンド目の記録 */
	bool HasBounced() const { return m_isHasBounced; }
	const Vector3& GetFirstBouncePos() const { return m_firstBouncePos; }

	/** 今の球がサーブかどうか */
	void SetServe(bool isServe) { m_isServe = isServe; }
	bool IsServe() const { return m_isServe; }

	/** ネットに引っかかったかどうか */
	bool IsHitNet() const { return m_isHitNet; }

	/** ネットが越えたかどうか */
	bool HasCrossedNet() const { return m_isCrossedNet; }

private:
	/** 最後に打った人 */
	const Character* m_lastHitPlayer = nullptr;

	/** ボールのモデル */
	ModelRender m_ballRender;

	/** ボールのポジション */
	Vector3 m_ballPosition = Vector3::Zero;
	
	/** ボールの速度 */
	Vector3 m_ballSpeed = Vector3::Zero;

	/** 1バウンド目の座標 */
	Vector3 m_firstBouncePos = Vector3::Zero;

	/** バウンドしたかどうか */
	bool m_isHasBounced = false;

	/** サーブの球かどうか */
	bool m_isServe = false;

	/** ボールを持っているかどうか */
	bool m_isHeld = true;

	/** ボールがネットに引っかかったかどうか */
	bool m_isHitNet = false;

	/** ボールがネットに越えたかどうか */
	bool m_isCrossedNet = false;

	float m_curveAccelX = 0.0f;
	float m_gravityScale = 1.0f;
	float m_bounceScale = 1.0f;

	/** ボールのバウンド回数 */
	uint8_t m_bounceCount = 0;
};

