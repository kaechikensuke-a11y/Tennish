#include "stdafx.h"
#include "Character.h"
#include "Ball/Ball.h"

namespace
{
	const char* BLUE_PLAYER_MODEL_PATH = "Assets/model/Player/TennisPlayerBlue.tkm";
	constexpr float PLAYER_POSITION_Y = -900.0f;

	constexpr float TOSS_SPEED  =  1500.0f; /** トスで上に投げる速さ */
	constexpr float SERVE_SPEED = 2000.0f; /** サーブ前方への速さ */
	constexpr float SERVE_UP    =  2500.0f; /** サーブ上向きの速さ */
	constexpr float HAND_HEIGHT =  350.0f; /** 足元から手までの高さ */
	constexpr float SWING_ENABLE_TIME = 0.3f; /** トス後、打てるようになるまでの時間 */

	/** Ball.cppと同じ値にする */
	constexpr float GRAVITY           = -2500.0f; /** 重力 */
	constexpr float GROUND_Y          = -680.0f; /** 地面の高さ */

	constexpr float SERVE_FLIGHT_TIME =    1.4f;/** 打ってから着地までの時間 */
	constexpr float SERVICE_BOX_X     =  550.0f;/** サービスボックス中央のX */
	constexpr float SERVICE_LINE_Z    = 5500.0f;/** 着地地点 */
	constexpr float AIM_RANGE_X       =  500.0f;/** スティック操作で狙いを決める */

	constexpr float NET_Z = 4000.0f; /** ネットのZ座標 */
	constexpr float SERVICE_LINE_DIST = 2000.0f; /** ネットからサービスラインまでの距離 */

	constexpr float HIT_RANGE         =  600.0f; /** ボールを打てる水平距離 */
	constexpr float HIT_MAX_HEIGHT	  = 1500.0f; /** 足元からこの高さまでなら打てる */
	constexpr float RALLY_FLIGHT_TIME =    1.3f; /** 打ち返してから着地するまでの時間 */
	constexpr float RALLY_AIM_X		  = 1000.0f; /** 左右の幅 */
	constexpr float RALLY_DIST_NEAR	  = 1500.0f; /** ネット寄りに落ちる */
	constexpr float RALLY_DIST_FAR	  = 3500.0f; /** ベースラインに落ちる */

	constexpr float RALLY_TIME_LOW = 1.8f;  /** 打点が低いときの飛行時間 */
	constexpr float RALLY_TIME_HIGH = 1.1f;  /** 打点が高いときの飛行時間 */
	constexpr float HIT_HEIGHT_LOW = 220.0f; /** 足元からこの高さ以下なら */
	constexpr float HIT_HEIGHT_HIGH = 1100.0f; /** 足元からこの高さ以上なら */
}

bool Character::Start()
{
	/** 子クラスが指定していればそれを使い、なければ既定のモデルを使う */
	const char* path = (m_modelPath != nullptr) ? m_modelPath : BLUE_PLAYER_MODEL_PATH;

	/** モデルの読み込み */
	m_modelRender.Init(path);

	/** 初期位置をモデルに反映 */
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	return true;
}

void Character::Update()
{
	/** どう動きたいかを子クラスから受け取る */
	Intent intent = DecideIntent();

	/** 経過時間 */
	float dt = g_gameTime->GetFrameDeltaTime();

	/** 移動できる状態かどうか */
	bool isMove = (m_serveState == ServeState::en_Waiting) ||
		(m_serveState == ServeState::en_Done);

	/** 移動できる状態なら */
	if (isMove)
	{
		/** 座標を動かす */
		m_position.x += intent.moveX * m_speed * dt;
		m_position.z += intent.moveZ * m_speed * dt;

		/** スティックを倒しているときだけ向きを変える */
		if (fabsf(intent.moveX) > 0.001f || fabsf(intent.moveZ) > 0.001f)
		{
			Vector3 dir(intent.moveX, 0.0f, intent.moveZ);
			m_rotation.SetRotationYFromDirectionXZ(dir);
		}
	}

	/** 高さは常に固定 */
	m_position.y = PLAYER_POSITION_Y;

	/** サーブ */
	UpdateServe(intent);

	/** ラリー(打ち返し) */
	UpdateRally(intent);

	/** 動かした座標をモデルに反映する */
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();
}

void Character::UpdateServe(const Intent& intent)
{
	/** ボールを探す */
	if (m_ball == nullptr)
	{
		m_ball = FindGO<Ball>("ball");
		if (m_ball == nullptr) return;
	}

	switch (m_serveState)
	{
	case ServeState::en_Waiting:
		/** ボールには触らない */
		break;
	case ServeState::en_Ready:
		/** ボールを手の位置に固定する */
		m_ball->SetHeld(true);
		m_ball->SetPosition(m_position + Vector3(0.0f, HAND_HEIGHT, 0.0f));
		
		/** ボタンでトス */
		if (intent.isSwing)
		{
			m_ball->SetHeld(false); /** 物理計算を再開 */
			m_ball->SetVelocity(Vector3(0.0f, TOSS_SPEED, 0.0f)); /** 真上に投げる */
			m_tossTimer = 0.0f; /** 経過時間を0から数える */
			m_serveState = ServeState::en_Tossed;
		}
		break;

	case ServeState::en_Tossed:
		/** トスしてからの経過時間を計算 */
		m_tossTimer += g_gameTime->GetFrameDeltaTime();

		/** もう一度ボタンで打つ */
		if (intent.isSwing && m_tossTimer > SWING_ENABLE_TIME)
		{
			/*
			 * 自分の立ち位置と対角線を狙う
			 * 自分が左にいれば+1,右にいれば-1にして対角線を狙う
			 */
			float diagonal = (m_position.x >= 0.0f) ? -1.0f : 1.0f;

			/** 着地地点を決める */
			Vector3 target;
			target.x = diagonal * SERVICE_BOX_X + intent.moveX * AIM_RANGE_X; /** 対角線ボックス中央+スティックで微調整 */
			target.y = GROUND_Y; /** 地面に着地させる */
			target.z = NET_Z + m_serveDirZ * (SERVICE_LINE_DIST * 0.6f); /** ネットから相手側へ */
			
			/** 打点からtargetへ1秒で着地する初速を逆算 */
			Vector3 vel = CalcHitVelocity(m_ball->GetPosition(), target, SERVE_FLIGHT_TIME);
			m_ball->SetVelocity(vel);

			/** トス中のバウンドを数えない */
			m_ball->ResetBounceCount();

			m_serveState = ServeState::en_Done;
		}
		break;

	case ServeState::en_Done:
		break;
	}
}

bool Character::IsBallComing() const
{
	if (m_ball == nullptr) return false;

	/** 自分の打つ方向と逆向きに飛んでいれば、こちらに向かってくる */
	return m_ball->GetVelocity().z * m_serveDirZ < 0.0f;
}

void Character::UpdateRally(const Intent& intent)
{
	if (m_ball == nullptr) return;

	/** 打てるのは、レシーブ待ち(最初の返球)かラリー中 */
	if (m_serveState != ServeState::en_Waiting &&
		m_serveState != ServeState::en_Done) return;

	if (!intent.isSwing) return;                    /** ボタンを押していない */
	if (!IsBallComing()) return;                    /** 相手の方へ飛んでいくボールは打てない */
	if (m_ball->GetBounceCount() >= 2) return;      /** 2バウンド後は打てない(ポイント終了) */

	/** ボールが届く範囲にあるか */
	Vector3 diff = m_ball->GetPosition() - m_position;
	float height = diff.y;
	diff.y = 0.0f;                         /** 水平距離だけで判定 */
	if (diff.Length() > HIT_RANGE) return; /** HIT_RANGEより遠かったら打ち返せない */
	if (height > HIT_MAX_HEIGHT) return;   /** HIT_MAX_HEIGHTより高いと届かない */

	/** 狙いの値を範囲内に収める */
	float aimX = intent.aimX;
	if (aimX > 1.0f) aimX = 1.0f;
	if (aimX < -1.0f) aimX = -1.0f;

	/** 着地させたい地点(ネット基準で、相手コートの中) */
	float distFromNet = RALLY_DIST_NEAR + (RALLY_DIST_FAR - RALLY_DIST_NEAR);

	Vector3 target;
	target.x = aimX * RALLY_AIM_X; /** 左右:狙い×幅 */
	target.y = GROUND_Y; /** 地面に着地させる */
	target.z = NET_Z + m_serveDirZ * distFromNet; /** 相手コートに打ち返す */

	/** 打点の高さ(足元基準)から飛行時間を決める */
	float t = (height - HIT_HEIGHT_LOW) / (HIT_HEIGHT_HIGH - HIT_HEIGHT_LOW);
	if (t < 0.0f) t = 0.0f;
	if (t > 1.0f) t = 1.0f;   /** 0:低い 〜 1:高い */

	float flightTime = RALLY_TIME_LOW + t * (RALLY_TIME_HIGH - RALLY_TIME_LOW);

	/** サーブと同じ逆算で打ち返す */
	Vector3 vel = CalcHitVelocity(m_ball->GetPosition(), target, RALLY_FLIGHT_TIME);
	m_ball->SetVelocity(vel);
	m_ball->ResetBounceCount();  /** 打ち返したのでバウンド数を戻す */

	/** レシーブ側もここからラリー中の扱いになる */
	m_serveState = ServeState::en_Done;
}

Vector3 Character::CalcHitVelocity(const Vector3& from, const Vector3& target, float flightTime) const
{
	Vector3 vel;

	/** 水平方向は重力が働かないので、速度 = 距離 / 時間 */
	vel.x = (target.x - from.x) / flightTime;
	vel.z = (target.z - from.z) / flightTime;

	vel.y = (target.y - from.y - 0.5f * GRAVITY * flightTime * flightTime) / flightTime;
	return vel;
}

void Character::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}