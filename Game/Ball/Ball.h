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
private:
	/** ボールのモデル */
	ModelRender m_ballRender;

	/** ボールのポジション */
	Vector3 m_ballPosition = Vector3::Zero;
	
	/** ボールの速度 */
	Vector3 m_ballSpeed = Vector3::Zero;

	/** ボールを持っているかどうか */
	bool m_isHeld = true;
};

