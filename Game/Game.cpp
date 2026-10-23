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

	const Vector3 PLAYER_SERVE_POS(1500.0f, 0.0f, -1000.0f);
	const Vector3 ENEMY_SERVE_POS(-1500.0f, 0.0f, 9000.0f);
}

bool Game::Start()
{
	NewGO<TennisCourt>(0, "tenniscourt");
	NewGO<GameCamera>(0, "gamecamera");
	m_player = NewGO<Player>(0, "player");
	m_player->SetPosition(Vector3(PLAYER_SERVE_POS));
	m_player->SetServeDirZ(1.0f);
	m_player->SetServer(true);
	
	m_enemy = NewGO<Enemy>(0, "enemy");
	m_enemy->SetPosition(Vector3(ENEMY_SERVE_POS));
	m_enemy->SetServeDirZ(-1.0f);
	m_enemy->SetServer(false);

	m_ball = NewGO<Ball>(0, "ball");

	m_debugFont.SetPosition(0.0f, 0.0f, 0.0f); // 画面左上あたり
	m_debugFont.SetScale(0.8f);
	m_debugFont.SetColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f)); // 黄色

	return true;
}

void Game::Update()
{
	if (m_ball == nullptr) return;

	wchar_t text[256];
	const Vector3& pp = m_player->GetPosition();
	const Vector3& bp = m_ball->GetPosition();

	const Character* lh = m_ball->GetLastHitPlayer();
	const wchar_t* who = (lh == m_player) ? L"Player" : (lh == m_enemy) ? L"Enemy" : L"None";
	const Vector3& fb = m_ball->GetFirstBouncePos();

	swprintf_s(text,
		L"Player X:%.0f Z:%.0f\nBall X:%.0f Y:%.0f Z:%.0f\nLastHitter:%s Serve:%d\nBounce:%d X:%.0f Z:%.0f\nNetHit:%d Crossed:%d",
		pp.x, pp.z, bp.x, bp.y, bp.z,
		who, m_ball->IsServe() ? 1 : 0,
		m_ball->HasBounced() ? 1 : 0, fb.x, fb.z,
		m_ball->IsHitNet() ? 1 : 0, m_ball->HasCrossedNet() ? 1 : 0);
	m_debugFont.SetText(text);
	


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
	m_debugFont.Draw(rc);
}