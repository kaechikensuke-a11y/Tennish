#include "stdafx.h"
#include "Game.h"
#include "Source/Character/Player/Player.h"
#include "Source/Character/Enemy/Enemy.h"
#include "Source/Stage/TennisCourt.h"
#include "GameCamera/GameCamera.h"
#include "Ball/Ball.h"
#include "Source/Score/ScoreManager.h"
#include "Source/Score/ScoreUI.h"
#include "Source/Court/CourtConst.h"
#include "Scene/Title/Title.h"
#include "Scene/Pause/PauseMenu.h"
#include "Scene/Option/Option.h"
#include "Scene/Pause/PauseState.h"
#include "nature/SkyCube.h"

namespace
{
	/** ポイント終了後から次のサーブに移るまでの時間 */
	constexpr float POINT_RESET_WAIT = 1.5f;

	/** サーブ位置: コート中心からの横のずれ */
	constexpr float SERVE_OFFSET_X = 1500.0f;

	/** サーバーが立つZ */
	constexpr float PLAYER_SERVE_Z = -1000.0f;
	constexpr float ENEMY_SERVE_Z = 9000.0f;
}

Game::~Game()
{
	PauseState::Reset();

	if (m_pauseMenu != nullptr) DeleteGO(m_pauseMenu);
	if (m_option != nullptr)    DeleteGO(m_option);

	m_matchEnd.Finish();
	DeleteGO(m_scoreUI);
	DeleteGO(m_skyCube);
	DeleteGO(m_ball);
	DeleteGO(m_player);
	DeleteGO(m_enemy);
	DeleteGO(m_gameCamera);
	DeleteGO(m_court);
}

bool Game::Start()
{
	/** 前のゲームがポーズ中に終わっていても、止まったまま始まらないようにする */
	PauseState::Reset();
	g_renderingEngine->SetDirectionLight(0, Vector3(-0.3f, -1.0f, 0.4f), Vector3(0.6f, 0.6f, 0.6f));
	
	/** 空を作る */
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetType(enSkyCubeType_DayToon);
	m_skyCube->SetLuminance(1.0f);
	m_skyCube->SetScale(1800.0f);
	m_skyCube->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

	m_court = NewGO<TennisCourt>(0, "tenniscourt");
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	m_player = NewGO<Player>(0, "player");
	m_player->SetServeDirZ(1.0f);
	m_player->SetServer(true);
	
	m_enemy = NewGO<Enemy>(0, "enemy");

	m_enemy->SetServeDirZ(-1.0f);
	m_enemy->SetServer(false);

	m_ball = NewGO<Ball>(0, "ball");

	m_debugFont.SetPosition(0.0f, 0.0f, 0.0f); // 画面左上あたり
	m_debugFont.SetScale(0.8f);
	m_debugFont.SetColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f)); // 黄色

	/** スコア管理と表示 */
	MatchSettings ms;
	ms.gamesToWin = 1;
	m_scoreManager = std::make_unique<ScoreManager>(ms);

	m_scoreUI = NewGO<ScoreUI>(0, "scoreui");
	m_scoreUI->SetScoreManager(m_scoreManager.get());

	StartNextPoint();
	return true;
}

void Game::Update()
{
	if (m_ball == nullptr) return;

	/** 再開待ちのフレームを1つ進める */
	PauseState::Tick();

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
		m_ball->IsHasBounced() ? 1 : 0, fb.x, fb.z,
		m_ball->IsHitNet() ? 1 : 0, m_ball->HasCrossedNet() ? 1 : 0,
		winnerName, reasonName, scoreText);
	m_debugFont.SetText(text);

	/** ポーズ中は、ポイント判定やタイマーを動かさずここで終わる */
	if (m_state == GameState::en_Pause || m_state == GameState::en_PauseOption)
	{
		UpdatePause();
		return;
	}

	/** 試合中にセレクトボタンでポーズを開く */
	if (m_state == GameState::en_Playing && g_pad[0]->IsTrigger(enButtonSelect))
	{
		OpenPause();
		return;
	}

	/** 試合終了の演出中は、ポイント処理を止める */
	if (m_state == GameState::en_MatchEnd)
	{
		m_matchEnd.Update();

		if (m_matchEnd.IsExitReady())
		{
			if (m_matchEnd.GetDecided() == 0)
			{
				/** リトライ */
				NewGO<Game>(0, "game");
			}
			else
			{
				/** タイトルへ */
				NewGO<Title>(0, "game");
			}
			DeleteGO(this);
		}
		return;
	}

	/** トスを上げたのに打てずに落ちた(サーブミス判定) */
	if (!m_pointEnded)
	{
		const bool playerServes = (m_scoreManager->GetServer() == ScoreManager::PLAYER);
		Character* server = playerServes ? static_cast<Character*>(m_player) : static_cast<Character*>(m_enemy);

		if (server->IsTossMissed())
		{
			m_pointEnded = true;
			m_resetTimer = 0.0f;

			if (m_serveCount == 1)
			{
				/** 一度目は打ちなおすことが出来るようにする */
				m_serveCount = 2;
				m_isRetry = true;
				m_pointResult.reason = PointReason::en_Fault;
			}
			else
			{
				/** 2回目 */
				m_pointResult.reason = PointReason::en_DoubleFault;
				m_pointResult.winner = playerServes ? static_cast<Character*>(m_enemy) : static_cast<Character*>(m_player);
				
				int winner = playerServes ? ScoreManager::ENEMY : ScoreManager::PLAYER;
				m_scoreManager->AddPoint(winner);
			
			}
		}
	}


	/** ポイント終了を検知 */
	if (!m_pointEnded)
	{
		PointResult r = m_pointJudge.Judge(*m_ball, m_player, m_enemy, 1.0f);

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

	/** 試合が決まったら、次のポイントへ進まず演出へ */
	if (m_scoreManager->IsMatchOver())
	{
		m_state = GameState::en_MatchEnd;
		m_matchEnd.Start(*m_scoreManager,m_player, m_enemy,m_ball,m_gameCamera);
		return;
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

Vector3 Game::CalcServerPos(bool isPlayer) const
{
	/** 偶数ポイントは右コートに、奇数ポイントは左コートにうつ */
	const bool rightCourt = (m_scoreManager->GetPointsPlayedInGame() % 2 == 0);

		float side = rightCourt ? 1.0f : -1.0f;
		if (!isPlayer) side = -side;

		const float z = isPlayer ? PLAYER_SERVE_Z : ENEMY_SERVE_Z;
		return Vector3(CourtConst::COURT_CENTER_X + side * SERVE_OFFSET_X, 0.0f, z);
}

void Game::StartNextPoint()
{
	/** サーブ権を交代 */
	/** 今のサーバーを ScoreManager から取得(ゲームが変わっていれば交代済み) */
	const bool playerServes = (m_scoreManager->GetServer() == ScoreManager::PLAYER);

	/** ボールをリセット */
	m_ball->ResetBounceCount();
	m_ball->SetVelocity(Vector3::Zero);
	m_ball->SetLastHitPlayer(nullptr);
	m_pointResult = PointResult();

	/** サーバーとレシーバーの位置 */
	Vector3 playerPos = CalcServerPos(true);
	Vector3 enemyPos = CalcServerPos(false);

	/** 打ち直しでなければ、1回目のサーブに戻す */
	if (!m_isRetry) m_serveCount = 1;
	m_isRetry = false;

	/** 位置とサーブ状態を戻す */
	m_player->ResetForServe(playerServes,playerPos);
	m_enemy->ResetForServe(!playerServes,enemyPos);

	m_pointEnded = false;
}

void Game::OpenPause()
{
	PauseState::Pause();
	m_pauseMenu = NewGO<PauseMenu>(1, "pausemenu");
	m_state = GameState::en_Pause;
}

void Game::UpdatePause()
{
	/** 設定画面を開いている間は、閉じるのを待つ */
	if (m_state == GameState::en_PauseOption)
	{
		if (m_option != nullptr && m_option->IsClosed())
		{
			DeleteGO(m_option);
			m_option = nullptr;

			/** ポーズ画面に戻る */
			m_pauseMenu = NewGO<PauseMenu>(1, "pausemenu");
			m_state = GameState::en_Pause;
		}
		return;
	}

	/** ポーズ画面の決定を受け取る */
	if (m_pauseMenu == nullptr) return;

	const int decided = m_pauseMenu->GetDecided();
	if (decided == -1) return;

	DeleteGO(m_pauseMenu);
	m_pauseMenu = nullptr;

	switch (decided)
	{
	case PauseMenu::en_Resume:
		/** ゲームに戻る */
		PauseState::Resume();
		m_state = GameState::en_Playing;
		break;

	case PauseMenu::en_Option:
		/** 設定画面を開く */
		m_option = NewGO<Option>(2, "option");
		m_state = GameState::en_PauseOption;
		break;

	case PauseMenu::en_Title:
		/** タイトルへ。Game を消すと、中のオブジェクトもデストラクタで消える */
		NewGO<Title>(0, "title");
		DeleteGO(this);
		break;

	default:
		break;
	}
}

void Game::Render(RenderContext& rc)
{
	//m_debugFont.Draw(rc);
}