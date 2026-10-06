#pragma once
class Ball;
/** 誰が操作しても同じ処理をまとめる */
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

	/** fromからtargetへflightTime秒で着地するための初速を計算して返す */
	Vector3 CalcHitVelocity(const Vector3& from, const Vector3& target, float flightTime) const;

	/*
	 * サーブ権の設定(生成直後) 
	 * trueならサーブをする
	 * falseならサーブを待つ
	 */
	void SetServer(bool isServer)
	{
		m_serveState = isServer ? ServeState::en_Ready : ServeState::en_Waiting;
	}

	/*
	 * 打つ方向を決める 
	 * +1なら奥に打つ、-1なら手前側に打つ
	 */
	void SetServeDirZ(float dirZ) { m_serveDirZ = dirZ; }

	/** サーブ権と位置をリセットして、次のポイントを始める */
	void ResetForServe(bool isServer, const Vector3& pos)
	{
		m_position = pos; /** 立ち位置を戻す */
		m_serveState = isServer ? ServeState::en_Ready : ServeState::en_Waiting;/** サーブ権に応じた状態に */
		m_tossTimer = 0.0f;/** 前のトスが上がる時間を初期化 */
	}

/** 自分と子クラスのみ使える */
protected:

	/** サーブの状態 */
	enum class ServeState
	{
		en_Waiting,/** サーブが打たれるのを待つ */
		en_Ready,  /** 構え、ボールを手に持っている */
		en_Tossed, /** トス中、もう一度ボタンで打つ */
		en_Done    /** サーブ完了(ここからラリーを開始する) */
	};

	/** どう動きたいかをまとめたもの */
	struct Intent
	{
		float moveX  = 0.0f; /** 左、右 */
		float moveZ  = 0.0f; /** 奥、手前 */
		bool isSwing = false; /** ボタンを押したかどうか */
	};

	/** サーブの状態を1フレーム進める */
	void UpdateServe(const Intent& intent);

	ServeState m_serveState = ServeState::en_Ready; /** 今のサーブ状態 */
	Ball* m_ball = nullptr; /** ボールへのポインタ */
	float m_serveDirZ = 1.0f; /** 相手コートの方向 */

	/** 返したIntentをもとにCharacterが動く */
	virtual Intent DecideIntent() = 0;

	/** キャラクターの移動量(１秒あたり) */
	float m_speed = 1500.0f; 

	/** トスしている時間 */
	float m_tossTimer = 0.0f;

	/** モデルのファイルパス(子クラスで差し替える) */
	const char* m_modelPath = nullptr;

	ModelRender m_modelRender;
	/** キャラクターの位置 */
	Vector3 m_position;
	/** キャラクターの向き */
	Quaternion m_rotation;

};

