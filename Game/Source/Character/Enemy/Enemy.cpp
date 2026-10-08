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
		}
		break;

	case ServeState::en_Tossed:
		/** トスが上がった後に打つ */
		m_npcTimer += dt;
		if (m_npcTimer > NPC_HIT_DELAY)
		{
			/** 時間経過で打つ */
			intent.isSwing = true;
			m_npcTimer = 0.0f;
		}
		break;
	case ServeState::en_Done:
		ChaseBall(intent); /** ラリー中 */
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
	diff.y = 0.0f;
	if (diff.Length() > NPC_STOP_DIST)
	{
		diff.Normalize();
		intent.moveX = diff.x;
		intent.moveZ = diff.z;
	}

	/** ボールが近ければスイング(届かなければ Character 側で無視される) */
	Vector3 toBall = m_ball->GetPosition() - m_position;
	toBall.y = 0.0f;
	if (toBall.Length() < NPC_SWING_RANGE)
	{
		intent.isSwing = true;

		/** 狙いを決める */
		intent.aimX = (rand() % 100 - 50) / 60.0f;  /** 約 -0.8〜0.8 */
	}
}