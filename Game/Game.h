#pragma once
#include <memory>
#include "Source/Score/ScoreManager.h"
#include "Level3DRender/LevelRender.h"
#include "Source/Judge/PointJudge.h"
class Player;
class Enemy;
class Ball;
class ScoreUI;

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

	void StartNextPoint();

	/** 生成したオブジェクトの保存 */
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	Ball* m_ball = nullptr;

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

