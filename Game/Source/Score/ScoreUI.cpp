#include "stdafx.h"
#include "ScoreUI.h"

namespace
{
	/** 画像のパス */
	const char* POINT_PATH[(int)PointView::en_Num] =
	{
		"Assets/UI/Score/zero.dds",
		"Assets/UI/Score/fifteen.dds",
		"Assets/UI/Score/thirty.dds",
		"Assets/UI/Score/forty.dds",
		"Assets/UI/Score/Adv.dds",
		"Assets/UI/Score/Deuce.dds",
	};

	/** スコアボード全体の位置とサイズ */
	const Vector3 BOARD_POS(0.0f, 450.0f, 0.0f);
	constexpr float BOARD_W = 960.0f;
	constexpr float BOARD_H = 400.0f;

	/** 点数画像のサイズ */
	constexpr float POINT_W = 100.0f;
	constexpr float POINT_H = 100.0f;

	/** ボード中心からの点数の位置 */
	const Vector3 PLAYER_POINT_OFFSET(-200.0f, 0.0f, 0.0f);
	const Vector3 ENEMY_POINT_OFFSET(200.0f, 0.0f, 0.0f);

	/** Deuce専用の大きさと位置 */
	constexpr float DEUCE_W = 340.0f;
	constexpr float DEUCE_H = 140.0f;
	const Vector3 DEUCE_OFFSET(0.0f, -70.0f, 0.0f);

	/** Deuceの点滅 */
	constexpr float DEUCE_BLINK_SPEED = 2.5f;  /** 大きいほど速い。2.0で約3秒に1往復 */
	constexpr float DEUCE_ALPHA_MIN = 0.1f;  /** 一番薄いときの透明度 */
	constexpr float DEUCE_ALPHA_MAX = 1.0f;  /** 一番濃いときの透明度 */
}

bool ScoreUI::Start()
{
	m_board.Init("Assets/UI/Score/ScoreBoard.dds",BOARD_W,BOARD_H);
	m_board.SetPosition(BOARD_POS);
	m_board.Update();

	for (int i = 0; i < (int)PointView::en_Num; i++)
	{
		if (i == (int)PointView::en_Deuce)
		{
			/** Deuceは中央に1枚だけ出す。Enemy用は作らない */
			m_points[i].Init(POINT_PATH[i], DEUCE_W, DEUCE_H);
			m_points[i].SetPosition(BOARD_POS + DEUCE_OFFSET);
			m_points[i].Update();
			continue;
		}

		m_points[i].Init(POINT_PATH[i], POINT_W, POINT_H);
		m_points[i].SetPosition(BOARD_POS + PLAYER_POINT_OFFSET);
		m_points[i].Update();

		m_pointsEnemy[i].Init(POINT_PATH[i], POINT_W, POINT_H);
		m_pointsEnemy[i].SetPosition(BOARD_POS + ENEMY_POINT_OFFSET);
		m_pointsEnemy[i].Update();
	}

	return true;
}

void ScoreUI::Update()
{
	if (m_score == nullptr) return;

	/** Deuceのときだけ点滅を進める(それ以外は先頭から始める) */
	if (m_score->GetPointView(ScoreManager::PLAYER) == PointView::en_Deuce)
	{
		m_deuceTimer += g_gameTime->GetFrameDeltaTime();

		/** sinで0〜1を往復させ、最小〜最大の範囲に変換する */
		float wave = (sinf(m_deuceTimer * DEUCE_BLINK_SPEED) + 1.0f) * 0.5f;
		float alpha = DEUCE_ALPHA_MIN + wave * (DEUCE_ALPHA_MAX - DEUCE_ALPHA_MIN);

		m_points[(int)PointView::en_Deuce].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
	}
	else
	{
		m_deuceTimer = 0.0f;
	}
}

void ScoreUI::Render(RenderContext& rc)
{
	if (m_score == nullptr) return;

	m_board.Draw(rc);

	PointView pv = m_score->GetPointView(ScoreManager::PLAYER);
	PointView ev = m_score->GetPointView(ScoreManager::ENEMY);

	if (pv == PointView::en_Deuce)
	{
		/** 両者40以降の同点: 左右に40を出し、中央にDeuceを足す */
		m_points[(int)PointView::en_40].Draw(rc);
		m_pointsEnemy[(int)PointView::en_40].Draw(rc);
		m_points[(int)PointView::en_Deuce].Draw(rc);
	}
	else
	{
		m_points[(int)pv].Draw(rc);
		m_pointsEnemy[(int)ev].Draw(rc);
	}

}