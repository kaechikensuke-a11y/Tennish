#pragma once

#include "Level3DRender/LevelRender.h"
class Player;
class Enemy;
class Ball;

class Game : public IGameObject
{
public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:

	void StartNextPoint();

	/** 生成したオブジェクトの保存 */
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	Ball* m_ball = nullptr;

	/** 現在のサーブ権 */
	bool m_playerServing = true;
	float m_resetTimer = 0.0f;
	bool m_pointEnded = false;

};

