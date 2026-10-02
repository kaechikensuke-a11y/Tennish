#pragma once
class Character : public IGameObject
{
public:
	Character(){}
	/** 継承 */
	virtual ~Character(){}

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/** モデルの生成後に初期位置を決める */
	void SetPosition(const Vector3& pos) { m_position = pos; }
/** 自分と子クラスのみ使える */
protected:
	struct Intent
	{
		float moveX = 0.0f; /** 左、右 */
		float moveZ = 0.0f; /** 奥、手前 */
	};

	/** どう動きたいかを決める関数 */
	virtual Intent DecideIntent() = 0;

	float m_X = 0.0f; /** X座標 */
	float m_Z = 0.0f; /** Y座標 */
	float m_speed = 200.0f; /** キャラクターの移動量(１秒あたり) */

	ModelRender m_modelRender;
	Vector3 m_position;

};

