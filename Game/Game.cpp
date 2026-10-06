#include "stdafx.h"
#include "Game.h"
#include "Source/Character/Player/Player.h"
#include "Source/Character/Enemy/Enemy.h"
#include "Source/Stage/TennisCourt.h"
#include "GameCamera/GameCamera.h"
#include "Ball/Ball.h"

namespace
{
	/** 2バウンドで終了 */
	constexpr uint8_t POINT_END_BOUNCE = 2;
	/** ポイント終了後から次のサーブに移るまでの時間 */
	constexpr float POINT_RESET_WAIT = 1.5f;
}

bool Game::Start()
{
	NewGO<TennisCourt>(0, "tenniscourt");
	NewGO<GameCamera>(0, "gamecamera");
	m_player = NewGO<Player>(0, "player");
	m_player->SetPosition(Vector3(1500.0f, 0.0f,-1000.0f));/** サーブの位置 */
	m_player->SetServeDirZ(1.0f);
	m_player->SetServer(true);
	
	m_enemy = NewGO<Enemy>(0, "enemy");
	m_enemy->SetPosition(Vector3(-1500.0f, 0.0f, 9000.0f));/** サーブの位置 */
	m_enemy->SetServeDirZ(-1.0f);
	m_enemy->SetServer(false);

	m_ball = NewGO<Ball>(0, "ball");

	return true;
}

void Game::Update()
{
	if (m_ball == nullptr) return;

	/** ポイント終了を検知 */
	if (!m_pointEnded && m_ball->GetBounceCount() >= POINT_END_BOUNCE)
	{
		m_pointEnded = true;
		m_resetTimer = 0.0f;
	}

	/** 少し待ってから次のサーブへ */
	if (m_pointEnded)
	{
		m_resetTimer += g_gameTime->GetFrameDeltaTime();
		if (m_resetTimer >= POINT_RESET_WAIT)
		{
			StartNextPoint();
		}
	}
}

void Game::StartNextPoint()
{
	/** サーブ権を交代 */
	m_playerServing = !m_playerServing;

	/** ボールをリセット */
	m_ball->ResetBounceCount();
	m_ball->SetVelocity(Vector3::Zero);

	/** 位置とサーブ状態を戻す */
	m_player->ResetForServe(m_playerServing, Vector3(1500.0f, 0.0f, -1000.0f));
	m_enemy->ResetForServe(!m_playerServing, Vector3(-1500.0f, 0.0f, 9000.0f));

	m_pointEnded = false;
}

void Game::Render(RenderContext& rc)
{

}