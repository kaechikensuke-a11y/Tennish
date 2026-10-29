#include "stdafx.h"
#include "Game.h"
#include "Source/Character/Player/Player.h"
#include "Source/Character/Enemy/Enemy.h"
#include "Source/Stage/TennisCourt.h"
#include "GameCamera/GameCamera.h"
#include "Ball/Ball.h"
#include "Source/Score/ScoreManager.h"
#include "Source/Score/ScoreUI.h"

namespace
{
	/** ポイント終了後から次のサーブに移るまでの時間 */
	constexpr float POINT_RESET_WAIT = 1.5f;

	const Vector3 PLAYER_SERVE_POS(1500.0f, 0.0f, -1000.0f);
	const Vector3 ENEMY_SERVE_POS(-1500.0f, 0.0f, 9000.0f);
}

Game::~Game()
{
	DeleteGO(m_scoreUI);
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

	/** スコア管理と表示 */
	MatchSettings ms;
	ms.gamesToWin = 3;
	m_scoreManager = std::make_unique<ScoreManager>(ms);

	m_scoreUI = NewGO<ScoreUI>(0, "scoreui");
	m_scoreUI->SetScoreManager(m_scoreManager.get());

	return true;
}

void Game::Update()
{
	if (m_ball == nullptr) return;

	wchar_t text[512];
	const Vector3& pp = m_player->GetPosition();
	const Vector3& bp = m_ball->GetPosition();

	const Character* lh = m_ball->GetLastHitPlayer();
	const wchar_t* who = (lh == m_player) ? L"Player" : (lh == m_enemy) ? L"Enemy" : L"None";
	const Vector3& fb = m_ball->GetFirstBouncePos();

	const wchar_t* winnerName = L"-";
	if (m_pointResult.winner == m_player) winnerName = L"Player";
	else if (m_pointResult.winner == m_enemy) winnerName = L"Enemy";

	const wchar_t* reasonName = L"None";
	switch (m_pointResult.reason)
	{
	case PointReason::en_Out:       reasonName = L"Out"; break;
	case PointReason::en_Net:       reasonName = L"Net"; break;
	case PointReason::en_OwnCourt:  reasonName = L"OwnCourt"; break;
	case PointReason::en_TwoBounce: reasonName = L"TwoBounce"; break;
	default: break;
	}

	wchar_t scoreText[128];
	swprintf_s(scoreText, L"Score %hs-%hs  Games %d-%d  Server:%d",
		m_scoreManager->GetPointText(ScoreManager::PLAYER),
		m_scoreManager->GetPointText(ScoreManager::ENEMY),
		m_scoreManager->GetGames(ScoreManager::PLAYER),
		m_scoreManager->GetGames(ScoreManager::ENEMY),
		m_scoreManager->GetServer());
	swprintf_s(text,
		L"Player X:%.0f Z:%.0f\nBall X:%.0f Y:%.0f Z:%.0f\nLastHitter:%s Serve:%d\nBounce:%d X:%.0f Z:%.0f\nNetHit:%d Crossed:%d\nWinner:%s Reason:%s\n%s",
		pp.x, pp.z, bp.x, bp.y, bp.z,
		who, m_ball->IsServe() ? 1 : 0,
		m_ball->HasBounced() ? 1 : 0, fb.x, fb.z,
		m_ball->IsHitNet() ? 1 : 0, m_ball->HasCrossedNet() ? 1 : 0,
		winnerName, reasonName, scoreText);
	m_debugFont.SetText(text);


	/** ポイント終了を検知 */
	if (!m_pointEnded)
	{
		PointResult r = m_pointJudge.Judge(*m_ball, m_player, m_enemy, 1.0f);
		if (r.isEnded)
		{
			m_pointResult = r;
			m_pointEnded = true;
			m_resetTimer = 0.0f;
		}

		if (r.isEnded)
		{
			m_pointResult = r;
			m_pointEnded = true;
			m_resetTimer = 0.0f;

			/** 勝者に点を加える */
			int winner = (r.winner == m_player) ? ScoreManager::PLAYER : ScoreManager::ENEMY;
			m_scoreManager->AddPoint(winner);
		}
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
	m_ball->SetLastHitPlayer(nullptr);
	m_pointResult = PointResult();

	/** 位置とサーブ状態を戻す */
	m_player->ResetForServe(m_playerServing, Vector3(1500.0f, 0.0f, -1000.0f));
	m_enemy->ResetForServe(!m_playerServing, Vector3(-1500.0f, 0.0f, 9000.0f));

	m_pointEnded = false;
}

void Game::Render(RenderContext& rc)
{
	m_debugFont.Draw(rc);
}