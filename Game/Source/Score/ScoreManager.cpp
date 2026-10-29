#include "stdafx.h"
#include "ScoreManager.h"

namespace
{
	/** ポイント数に対応する表示 */
	const char* POINT_TEXT[4] = { "0","15","30","40" };
}

ScoreManager::ScoreManager(const MatchSettings& settings) : m_settings(settings)
{
	Reset();
}

void ScoreManager::Reset()
{
	m_points[PLAYER] = m_points[ENEMY] = 0;
	m_games[PLAYER]  = m_games[ENEMY] = 0;
	m_server         = PLAYER;
	m_matchWinner    = -1;
	m_lastGameWinner = -1;
}

ScoreEvent ScoreManager::AddPoint(int winner)
{
	/** 試合が終わっていたら何もしない */
	if (IsMatchOver()) return ScoreEvent::en_Match;

	const int loser = 1 - winner;
	m_points[winner]++;

	/** 4ポイント且つ2ポイント差 */
	const bool gameWon = (m_points[winner] >= 4) &&
		(m_points[winner] - m_points[loser] >= 2);

	if (!gameWon) return ScoreEvent::en_Point;

	/** ゲーム終了 */
	m_games[winner]++;
	m_lastGameWinner = winner;
	m_points[PLAYER] = m_points[ENEMY] = 0;

	/** 1ゲーム終了ごとにサーブ権を交代させる */
	m_server = 1 - m_server;

	/** 先取ゲーム数に達したら試合終了 */
	if (m_games[winner] >= m_settings.gamesToWin)
	{
		m_matchWinner = winner;
		return ScoreEvent::en_Match;
	}
	return ScoreEvent::en_Game;
}

bool ScoreManager::IsDeuce() const
{
	return m_points[PLAYER] >= 3 && m_points[PLAYER] == m_points[ENEMY];
}

const char* ScoreManager::GetPointText(int player) const
{
	const int mine  = m_points[player];
	const int other = m_points[1 - player];

	/** 両者が3ポイント以上(40-40以降) */
	if (mine >= 3 && other >= 3)
	{
		if (mine == other) return "40";
		if (mine > other)  return "AD";
		return "40";
	}

	return POINT_TEXT[mine];
}

PointView ScoreManager::GetPointView(int player) const
{
	const int mine  = m_points[player];
	const int other = m_points[1 - player];

	/** 両者が3ポイント以上(40-40以降のことを指す) */
	if (mine >= 3 && other >= 3)
	{
		if (mine == other) return PointView::en_Deuce;
		if (mine > other) return PointView::en_Adv;
		return PointView::en_40;
	}

	switch (mine)
	{
	case 0: return PointView::en_0;
	case 1: return PointView::en_15;
	case 2: return PointView::en_30;
	default: return PointView::en_40;
	}
}