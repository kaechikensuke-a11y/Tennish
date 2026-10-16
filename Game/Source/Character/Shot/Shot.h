#pragma once

/** ショットの種類 */
enum class ShotType
{
	en_Normal,
	en_BackSpin,
	en_TopSpin,
	en_Curve
};

/** チャージ段階が上がる時間(秒) */
constexpr float CHARGE_TIME_LV1 = 0.5f;
constexpr float CHARGE_TIME_LV2 = 1.0f;

/** 押している時間からチャージ段階を返す */
inline int CalcChargeLevel(float chargeTime)
{
	if (chargeTime >= CHARGE_TIME_LV2) return 2;
	if (chargeTime >= CHARGE_TIME_LV1) return 1;
	return 0;
}

/** ボールに渡す値 */
struct ShotParam
{
	float distFromNet = 0.0f;
	float flightTime = 1.0f;
	float gravityScale = 1.0f;
	float curveAccel = 0.0f;
	float bounceScale = 1.0f;
	float aimWidth = 0.0f;
};

/** ショット種類とチャージ段階から弾道パラメーターを作る */
/*
 * baseDist/baseTIme : 通常ショットの基準値(打点の高さなどで決めたもの)
 * aimX              : スティックの左右
 * myX               : 打つ人のX座標
 * (スティック入力せずにカーブで打ち返したときのみ右は左に、左は右に向かってか＾部刷る)
 */
ShotParam MakeShotParam(ShotType type, int level, float baseDist,
	float baseTime, float aimX, float myX);

