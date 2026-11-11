#pragma once
#include "Scene/Result/Fade/FadeScreen.h"

class Character;
class MatchResultUI;
class FadeScreen;
class ScoreManager;
class GameCamera;
class Ball;

class MatchEndDirector
{
public:
	/** 演出を始める */
	void Start(const ScoreManager& score,Character* player, Character* enemy, Ball* ball,GameCamera* camera);

	/** 演出中の更新 */
	void Update();

	void Finish();

private:
	/** ホワイトアウトの切り替え */
	void SwitchScene();

	enum class Phase
	{
		en_EndImage,
		en_FadeOut,
		en_FadeIn,
		en_Result,
	};

	float m_timer = 0.0f;

	Phase m_phase = Phase::en_EndImage;
	MatchResultUI* m_resultUI = nullptr;
	FadeScreen* m_fade = nullptr;
	const ScoreManager* m_score = nullptr;
	Character* m_player = nullptr;
	Character* m_enemy = nullptr;
	Ball* m_ball = nullptr;
	GameCamera* m_camera = nullptr;
};

