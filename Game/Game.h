#pragma once
#include <memory>
#include "Source/Score/ScoreManager.h"
#include "Level3DRender/LevelRender.h"
#include "Source/Judge/PointJudge.h"
#include "Scene/Result/MatchEndDirector.h"
class Player;
class Enemy;
class Ball;
class ScoreUI;
class GameCamera;
class TennisCourt;
class PauseMenu;
class Option;
class SkuCube;

class Game : public IGameObject
{
public:
	Game() {}
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	FontRender m_debugFont;

private:
	enum class GameState
	{
		en_Playing, /** 試合中 */
		en_Pause,        /** ポーズ画面を表示中 */
		en_PauseOption,  /** ポーズから開いた設定画面を表示中 */
		en_MatchEnd,/** 試合終了の演出中 */
	};

	GameState m_state = GameState::en_Playing;
	MatchEndDirector m_matchEnd;

	void StartNextPoint();

	/** ポーズ画面を開く */
	void OpenPause();

	/** ポーズ中の処理(ポーズ画面の決定を受け取って動かす) */
	void UpdatePause();

	/** 生成したオブジェクトの保存 */
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	Ball* m_ball = nullptr;
	TennisCourt* m_court = nullptr;
	GameCamera* m_gameCamera = nullptr;
	PauseMenu* m_pauseMenu = nullptr;
	Option* m_option = nullptr;
	SkyCube* m_skyCube = nullptr;

	PointJudge  m_pointJudge;
	PointResult m_pointResult;

	/** 現在のサーブ権 */
	float m_resetTimer = 0.0f;
	bool  m_pointEnded = false;

	/** サーブミスのバウンド回数 */
	int  m_serveCount = 1;
	/** サーブミスの打ち直しかどうか */
	bool m_isRetry    = false;

	/** 今のポイント数から、サーバー側の立ち位置を返す */
	Vector3 CalcServerPos(bool isPlayer) const;

	/** スコア管理 */
	std::unique_ptr<ScoreManager>m_scoreManager;

	/** スコア表示 */
	ScoreUI* m_scoreUI = nullptr;

};

