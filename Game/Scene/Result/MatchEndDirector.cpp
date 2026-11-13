#include "stdafx.h"
#include "MatchEndDirector.h"
#include "MatchResultUI.h"
#include "Source/Character/Character.h"
#include "Source/Score/ScoreManager.h"
#include "Ball/Ball.h"
#include "GameCamera/GameCamera.h"

namespace
{
	/** 白くなるまでの時間 */
	constexpr float FADE_OUT_TIME = 1.0f;

	const Vector3 WINNER_POS(600.0f, 0.0f, 1800.0f);
	const Vector3 LOSER_POS (-500.0f, 0.0f, 3400.0f);
	const Vector3 END_CAMERA_POS   (-100.0f, -200.0f, 300.0f);
	const Vector3 END_CAMERA_TARGET(-100.0f, -300.0f, 2500.0f);


	/** 白から戻るまでの時間 */
	constexpr float FADE_IN_TIME = 1.0f;
	/** 真っ白な画面のままの待ち時間 */
	constexpr float WHITE_HOLD = 0.3f;
}

void MatchEndDirector::Start(const ScoreManager& score,Character* player, Character* enemy,Ball* ball,GameCamera* camera)
{
	m_score = &score;
	m_player = player;
	m_enemy = enemy;
	m_ball = ball;
	m_camera = camera;

	/** 操作を止める */
	player->SetControlEnabled(false);
	enemy->SetControlEnabled(false);

	/** 終了画像を出す */
	m_resultUI = NewGO<MatchResultUI>(5, "matchresultui");
	m_resultUI->ShowEndImage();

	m_phase = Phase::en_EndImage;

	/** フェードを開始するクラス */
	m_fade = NewGO<FadeScreen>(10, "fadescreen");
}

void MatchEndDirector::Update()
{
	switch (m_phase)
	{
	case Phase::en_EndImage:
		/** 終了画像が消えたら、白いフェードアウトを始める */
		if (m_resultUI != nullptr && m_resultUI->IsEndImageFinished())
		{
			m_fade->StartFadeOut(FADE_OUT_TIME);
			m_phase = Phase::en_FadeOut;
		}
		break;

	case Phase::en_FadeOut:
		/** 真っ白になったら次へ移行する */
		if (m_fade->IsFinished())
		{
			SwitchScene();
			m_timer = 0.0f;
			m_phase = Phase::en_FadeIn;
		}
		break;

	case Phase::en_FadeIn:
		if (m_timer < WHITE_HOLD)
		{
			m_timer += g_gameTime->GetFrameDeltaTime();
			if (m_timer >= WHITE_HOLD)
			{
				m_fade->StartFadeIn(FADE_IN_TIME);
			}

		}
		else if (m_fade->IsFinished())
		{
			m_resultUI->ShowResult(
				m_score->GetGames(ScoreManager::PLAYER),
				m_score->GetGames(ScoreManager::ENEMY));
			m_phase = Phase::en_Result;
		}
		break;

	case Phase::en_Result:
		/** メニューで決定されたら、フェードアウトを始める */
		if (m_resultUI != nullptr)
		{
			const int d = m_resultUI->GetMenuDecided();
			if (d != -1)
			{
				m_decided = d;
				m_fade->StartFadeOut(FADE_OUT_TIME);
				m_phase = Phase::en_Exit;
			}
		}
		break;

	case Phase::en_Exit:
		if (m_fade->IsFinished())
		{
			m_exitReady = true;
		}
		break;
	}
}

void MatchEndDirector::Finish()
{
	if (m_resultUI != nullptr)
	{
		DeleteGO(m_resultUI);
		m_resultUI = nullptr;
	}

	if (m_fade != nullptr)
	{
		DeleteGO(m_fade);
		m_fade = nullptr;
	}
}

void MatchEndDirector::SwitchScene()
{
	const bool playerWon = (m_score->GetMatchWinner() == ScoreManager::PLAYER);
	Character* winner = playerWon ? m_player : m_enemy;
	Character* loser  = playerWon ? m_enemy : m_player;

	/** ボールを消す */
	m_ball->SetVisible(false);

	/** プレイヤーの配置と向きを変える */
	winner->SetPosition(WINNER_POS);
	loser->SetPosition(LOSER_POS);

	Quaternion rot;
	rot.SetRotationYFromDirectionXZ(Vector3(0.0f, 0.0f, -1.0f));
	winner->SetRotation(rot);
	loser->SetRotation(rot);

	/** カメラ */
	m_camera->SetCamera(END_CAMERA_POS, END_CAMERA_TARGET);
}