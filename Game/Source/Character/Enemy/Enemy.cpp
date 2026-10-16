#include "stdafx.h"
#include "Enemy.h"
#include "Ball/Ball.h"

namespace
{
	/** npcのモデル */
	const char* RED_PLAYER_MOSDEL_PATH = "Assets/model/Player/TennisPlayerRed.tkm";

	constexpr float NPC_TOSS_DELAY = 1.0f; /** サーブを構えてからトスするまでの待ち時間 */
	constexpr float NPC_HIT_DELAY = 0.65f;/** トスしてから打つまでの時間 */

	constexpr float NPC_STOP_DIST   = 50.0f;  /** 目標地点にこれだけ近づいたら止まる */
	constexpr float NPC_SWING_RANGE = 400.0f; /** この距離に入ったらスイング */

	constexpr int NPC_JUST_SERVE_ODDS = 10; /** 1/10 でジャストサーブ(将来は難易度で変える) */
}

Enemy::Enemy()
{
	m_modelPath = RED_PLAYER_MOSDEL_PATH;
}

Enemy::Intent Enemy::DecideIntent()
{
	Intent intent;

	/** 経過時間 */
	float dt = g_gameTime->GetFrameDeltaTime();

	switch (m_serveState)
	{
	case ServeState::en_Waiting:
		m_npcTimer = 0.0f;   /** 待機中にタイマーをリセットしておく */
		ChaseBall(intent);        /** レシーブ側として動く */
		break;

	case ServeState::en_Ready:

		/** 1秒後にトスを上げる */
		m_npcTimer += dt;
		if (m_npcTimer > NPC_TOSS_DELAY)
		{
			/** トス */
			intent.isSwing = true;
			m_npcTimer = 0.0f;

			/** トスする時点で、今回ジャストを狙うか決める */
			bool isJust = (rand() % NPC_JUST_SERVE_ODDS) == 0;
			m_npcHitDelay = isJust ? GetTossApexTime() : NPC_HIT_DELAY;
		}
		break;

	case ServeState::en_Tossed:
		/** トスからの経過時間(Character の m_tossTimer)で打つ */
		if (m_tossTimer > m_npcHitDelay)
		{
			intent.isSwing = true;
		}
		break;
	case ServeState::en_Done:
		ChaseBall(intent); /** ラリー中は追いかけて打つ */
		break;
	}

	return intent;
}

void Enemy::ChaseBall(Intent& intent)
{
	if (m_ball == nullptr) return;
	/** こちらに向かって飛んでいないときは何もしない */
	if (!IsBallComing()) return;

	/** バウンド前は着地予測地点へ先回り、バウンド後はボールそのものを追う */
	Vector3 dest = (m_ball->GetBounceCount() == 0)
		? m_ball->PredictLanding()
		: m_ball->GetPosition();

	/** 目標地点へ向かって移動 */
	Vector3 diff = dest - m_position;
	diff.y = 0.0f; /** 水平方向だけで考える */
	if (diff.Length() > NPC_STOP_DIST) /** 近すぎるとぶるぶる震えるため */
	{
		diff.Normalize();
		/** プレイヤーのスティック入力と同じ形式 */
		intent.moveX = diff.x;
		intent.moveZ = diff.z;
	}

	/** ボールが近ければスイング(届かなければ Character 側で無視される) */
	Vector3 toBall = m_ball->GetPosition() - m_position;
	toBall.y = 0.0f;

	if (toBall.Length() < NPC_SWING_RANGE)
	{
		intent.isSwing = true;
		intent.aimX = (rand() % 100 - 50) / 60.0f;

		/** 6割は通常、残りを他のショットに振り分ける */
		int r = rand() % 10;
		if (r < 6) intent.shot = ShotType::en_Normal;
		else if (r < 7) intent.shot = ShotType::en_BackSpin;
		else if (r < 9) intent.shot = ShotType::en_TopSpin;
		else            intent.shot = ShotType::en_Curve;
	}
	
}