#include "stdafx.h"
#include "Enemy.h"

namespace
{
	/** npcのモデル */
	const char* RED_PLAYER_MOSDEL_PATH = "Assets/model/Player/TennisPlayerRed.tkm";

	constexpr float NPC_TOSS_DELAY = 1.0f; /** サーブを構えてからトスするまでの待ち時間 */
	constexpr float NPC_HIT_DELAY = 0.65f;/** トスしてから打つまでの時間 */
}

Enemy::Enemy()
{
	m_modelPath = RED_PLAYER_MOSDEL_PATH;
}

Enemy::Intent Enemy::DecideIntent()
{
	Intent intent;

	float dt = g_gameTime->GetFrameDeltaTime();

	switch (m_serveState)
	{
	case ServeState::en_Ready:

		/** 1秒後にトスを上げる */
		m_npcTimer += dt;
		if (m_npcTimer > NPC_TOSS_DELAY)
		{
			intent.isSwing = true;
			m_npcTimer = 0.0f;
		}
		break;

	case ServeState::en_Tossed:
		/** トスが上がった後に打つ */
		m_npcTimer += dt;
		if (m_npcTimer > NPC_HIT_DELAY)
		{
			intent.isSwing = true;
			intent.moveX = ((rand() % 500) - 100) / 100.0f;
			m_npcTimer = 0.0f;
		}
		break;

	default:
		break;
	}

	return intent;
}