#pragma once
class Ball;
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

	/** ? */
	Vector3 CalcHitVelocity(const Vector3& from, const Vector3& target, float flightTime) const;
/** 自分と子クラスのみ使える */
protected:

	/** サーブの状態 */
	enum class ServeState
	{
		en_Ready,  /** 構え、ボールを手に持っている */
		en_Tossed, /** トス中、もう一度ボダンで打つ */
		en_Done    /** サーブ完了(ここからラリーを開始する) */
	};

	struct Intent
	{
		float moveX  = 0.0f; /** 左、右 */
		float moveZ  = 0.0f; /** 奥、手前 */
		bool isSwing = false; /** ボタンを押したかどうか */
	};

	void UpdateServe(const Intent& intent);

	ServeState m_serveState = ServeState::en_Ready;
	Ball* m_ball = nullptr;
	float m_serveDirZ = 1.0f; /** 相手コートの方向 */

	/** どう動きたいかを決める関数 */
	virtual Intent DecideIntent() = 0;

	/** キャラクターの移動量(１秒あたり) */
	float m_speed = 1500.0f; 

	/** トスしている時間 */
	float m_tossTimer = 0.0f;

	ModelRender m_modelRender;
	/** キャラクターの位置 */
	Vector3 m_position;
	/** キャラクターの向き */
	Quaternion m_rotation;

};

