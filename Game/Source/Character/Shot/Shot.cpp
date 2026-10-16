#include "stdafx.h"
#include "Shot.h"

namespace
{
	/** コートの左右幅の半分 */
	constexpr float COURT_HALF_WIDTH = 1550.0f;

	/** Bの通常ショットの曲がり幅 */
	constexpr float NORMAL_BULGE = 400.0f;

	/** Xの曲がり幅(チャージ段階別) */
	constexpr float CURVE_BULGE[3] = { 800.0f,1000.0f,1200.0f };

	/** スティックを倒した際の最小値 */
	constexpr float STICK_DEADZONE = 0.2f;

	/** ショットごとの倍率表 */
	struct ShotTable
	{
		float time[3];   /** 飛行時間の倍率(小さいほど速い) */
		float dist[3];	 /** 着地の深さの倍率 */
		float bounce[3]; /** 跳ね返りの倍率 */
		float gravity[3];/** 重力倍率(大きいほど高く上がって鋭く落ちる) */
		float aimRate;	 /** 左右に狙える幅 */
	};

	/**  */
	const ShotTable TABLES[4] =
	{
		/** B 通常 */
		{ {1.00f, 0.92f, 0.85f}, {1.00f, 1.00f, 1.05f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, 0.90f },
		/** A バックスピン: 遅い代わりに手前に落ちて跳ねない */
		{ {1.20f, 1.20f, 1.20f}, {0.50f, 0.40f, 0.30f}, {0.6f, 0.45f, 0.3f}, {1.0f, 1.0f, 1.0f}, 0.90f },
		/** Y ドライブ: 速くて高め。代わりに左右の幅が狭い */
		{ {0.70f, 0.63f, 0.56f}, {1.00f, 1.05f, 1.10f}, {1.0f, 1.1f, 1.2f}, {2.5f, 2.8f, 3.0f}, 0.55f },
		/** X カーブ: 曲がる代わりに遅め */
		{ {1.15f, 1.08f, 1.00f}, {1.00f, 1.00f, 1.00f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, 0.90f },
	};

}

ShotParam MakeShotParam(ShotType type, int level, float baseDist,
	float baseTime, float aimX, float myX)
{
	/** 段階を 0〜2 に収める */
	if (level < 0) level = 0;
	if (level > 2) level = 2;

	const ShotTable& t = TABLES[(int)type];

	ShotParam p;
	p.distFromNet = baseDist * t.dist[level];
	p.flightTime = baseTime * t.time[level];
	p.gravityScale = t.gravity[level];
	p.bounceScale = t.bounce[level];
	p.aimWidth = COURT_HALF_WIDTH * t.aimRate;

	/*
	 * 曲がり幅(bulge): 打ち出しの線と着地点の横のズレ。符号が曲がる向き。
	 * 横の移動量は x = vx*t + 1/2*a*t^2 なので、a = 2*bulge / t^2
	 */
	float bulge = 0.0f;
	switch (type)
	{
	case ShotType::en_Normal:
		/** スティックを倒した側へ、わずかに曲がる */
		bulge = NORMAL_BULGE * aimX;
		break;

	case ShotType::en_Curve:
	{
		float dir;
		if (fabsf(aimX) > STICK_DEADZONE)
		{
			/** スティックを倒した側へ曲がる */
			dir = (aimX > 0.0f) ? 1.0f : -1.0f;
		}
		else
		{
			/** 無入力: 右にいれば左へ、左にいれば右へ(コートの内側へ) */
			dir = (myX >= 0.0f) ? -1.0f : 1.0f;
		}
		bulge = CURVE_BULGE[level] * dir;
		break;
	}

	default: /** A と Y は曲がらない */
		break;
	}
	p.curveAccel = 2.0f * bulge / (p.flightTime * p.flightTime);

	return p;
}