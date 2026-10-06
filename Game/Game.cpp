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
	auto* player = NewGO<Player>(0, "player");
	player->SetPosition(Vector3(1500.0f, 0.0f,-1000.0f));/** サーブの位置 */
	player->SetServeDirZ(1.0f);
	player->SetServer(true);
	
	auto* enemy = NewGO<Enemy>(0, "enemy");
	enemy->SetPosition(Vector3(-1500.0f, 0.0f, 9000.0f));/** サーブの位置 */
	enemy->SetServeDirZ(-1.0f);
	enemy->SetServer(false);

	NewGO<Ball>(0, "ball");

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

		}
	}
}

void Game::Render(RenderContext& rc)
{

}