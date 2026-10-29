#pragma once

/** 試合の設定 */
struct MatchSettings
{
	int gamesToWin = 3;
};

/** ポイントが発生した結果どうなったか */
enum class ScoreEvent
{
	en_Point, /** ポイントが加算されるのみ */
	en_Game,  /** ゲームポイントが決まった */
	en_Match  /** マッチポイントが決まった */
};

/** 表示用の点数の状態 */
enum class PointView
{
	en_0,
	en_15,
	en_30,
	en_40,
	en_Adv,
	en_Deuce,
	en_Num
};

class ScoreManager
{
public:
	/** 人物の番号 */
	static constexpr int PLAYER = 0;
	static constexpr int ENEMY  = 1;

	explicit ScoreManager(const MatchSettings& settings);

	/** 試合を最初からやり直す */
	void Reset();

	/** 点を決めた方にポイントを加える */
	ScoreEvent AddPoint(int winner);

	/** 試合が終わったか */
	bool IsMatchOver() const { return m_matchWinner >= 0; }

	/** 試合の勝者(終わっていなければ -1) */
	int GetMatchWinner() const { return m_matchWinner; }

	/** 現在のサーブ権 */
	int GetServer() const { return m_server; }

	/** 今のゲームで、これまでに行われたポイントの合計(サーブ位置の左右に使う) */
	int GetPointsPlayedInGame() const { return m_points[PLAYER] + m_points[ENEMY]; }

	/** 表示用 */
	const char* GetPointText(int player) const;

	/** 同点(Deuceがどうか) */
	bool IsDeuce() const;

	/** 取得したゲーム数 */
	int GetGames(int player) const { return m_games[player]; }

	/** 直前のゲームに勝った人 */
	int GetLastGameWinner() const { return m_lastGameWinner; }

	/** 表示用の状態を返す */
	PointView GetPointView(int player) const;

private:
	MatchSettings m_settings;

	int m_points[2] = { 0,0 };
	int m_games[2]  = { 0,0 };
	int m_server    = PLAYER;
	int m_matchWinner    = -1;
	int m_lastGameWinner = -1;

};
