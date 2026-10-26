#include "stdafx.h"
#include "PointJudge.h"
#include "Ball/Ball.h"
#include "Source/Character/Character.h"
#include "Source/Court/CourtConst.h"

namespace
{
	/** 2バウンドで終了 */
	constexpr int POINT_END_BOUNCE = 2;
}

PointResult PointJudge::Judge(const Ball& ball, Character* player, Character* enemy,
	float playerDirZ) const
{
	PointResult result;

	const Character* hitter = ball.GetLastHitPlayer();
	
	/** まだ誰も打ってない */
	if (hitter == nullptr) return result;

	/** 打者と相手 */
	const bool hitterIsPlayer = (hitter == player);
	Character* hitterChar = hitterIsPlayer ? player : enemy;
	Character* other = hitterIsPlayer ? enemy : player;

	/** 打者が打つ方向 */
	const float dirZ = hitterIsPlayer ? playerDirZ : -playerDirZ;

	/** もしネットに引っかかったら */
	if (ball.IsHitNet())
	{
		result.isEnded = true;
		result.winner = other;
		result.reason = PointReason::en_Net;
		return result;
	}

	/** まだラリー中だったら何も起きない */
	if (!ball.HasBounced()) return result;

	const Vector3& fb = ball.GetFirstBouncePos();

	/** 1バウンドした際に自分コートか相手コートか判別させる */
	const bool inOpponentSide = (dirZ > 0.0f) ? (fb.z > CourtConst::NET_Z) : (fb.z < CourtConst::NET_Z);

	/** ライン内かどうか */
	const bool inX = (fb.x >= CourtConst::SIDELINE_LEFT_X) && (fb.x <= CourtConst::SIDELINE_RIGHT_X);
	const bool inZ = (fb.z >= CourtConst::BASELINE_Z_NEAR) && (fb.z <= CourtConst::BASELINE_Z_FAR);


	if (!inOpponentSide)
	{
		result.isEnded = true;
		result.winner = other;
		result.reason = PointReason::en_OwnCourt;
		return result;
	}

	if (!inX || !inZ)
	{
		result.isEnded = true;
		result.winner = other;
		result.reason = PointReason::en_Out;
		return result;
	}

	/** 範囲内に打ち返し2バウンドしたら打者の勝利 */
	if (ball.GetBounceCount() >= POINT_END_BOUNCE)
	{
		result.isEnded = true;
		result.winner = hitterChar;
		result.reason = PointReason::en_TwoBounce;
	}

	return result;
}
