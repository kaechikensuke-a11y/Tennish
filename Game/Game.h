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
	bool m_playerServing = true;
	float m_resetTimer = 0.0f;
	bool m_pointEnded = false;

	/** スコア管理 */
	std::unique_ptr<ScoreManager>m_scoreManager;

	/** スコア表示 */
	ScoreUI* m_scoreUI = nullptr;

};

