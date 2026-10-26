#pragma once
class Ball;
class Character;

/** ポイントが決まった理由 */
enum class PointReason
{
	en_None,      
	en_Out,		  /**       ライン外       */
	en_Net,		  /**   ネットにかかった   */
	en_OwnCourt,  /** 自分のコートに落ちた */
	en_TwoBounce, /**  相手が返せなかった  */
};

/** 判定結果 */
struct PointResult
{
	/** ポイントが決まったかどうか */
	bool isEnded = false;

	/** 勝者 */
	Character* winner = nullptr;

	PointReason reason = PointReason::en_None;
};

class PointJudge
{
public:
	/** 判定する */
	PointResult Judge(const Ball& ball, Character* player, Character* enemy, float playerDirZ) const;
};

