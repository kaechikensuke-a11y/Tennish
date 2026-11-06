#pragma once
class Character;
class MatchResultUI;
class MatchEndDirector
{
public:
	/** 演出を始める */
	void Start(Character* player, Character* enemy);

	/** 演出中の更新 */
	void Update();

	void Finish();

private:
	MatchResultUI* m_resultUI = nullptr;
};

