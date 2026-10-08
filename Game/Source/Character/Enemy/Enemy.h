#pragma once
#include "Source/Character/Character.h"
class Enemy : public Character
{
public:
	Enemy();
protected:
	Intent DecideIntent() override;

private:
	/** npcがトスしてから打つまでの時間 */
	float m_npcTimer = 0.0f;

	/** ボールを追いかけて、近づいたら打つ */
	void ChaseBall(Intent& intent);
};

