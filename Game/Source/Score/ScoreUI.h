#pragma once
#include "Source/Score/ScoreManager.h"
class ScoreUI : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetScoreManager(const ScoreManager* score) { m_score = score; }
private:
	const ScoreManager* m_score = nullptr;

	/** Deuce点滅用の経過時間 */
	float m_deuceTimer = 0.0f;

	SpriteRender m_board;
	SpriteRender m_points[(int)PointView::en_Num];
	SpriteRender m_pointsEnemy[(int)PointView::en_Num];
};

