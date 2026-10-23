#pragma once

namespace CourtConst
{
	/** 重力 */
	constexpr float GRAVITY  = -1200.0f;

	/** ボールがバウンドする座標 */
	constexpr float GROUND_Y =  -680.0f;

	/** コートの中心のX */
	constexpr float COURT_CENTER_X = -100.0f;

	/** シングルスのサイドライン */
	constexpr float SIDELINE_LEFT_X  = -1750.0f;
	constexpr float SIDELINE_RIGHT_X =  1550.0f;

	/** ベースライン */
	constexpr float BASELINE_Z_NEAR = -600.0f; //自陣側
	constexpr float BASELINE_Z_FAR  = 8300.0f; //相手側

	/** サービスライン */
	constexpr float SERVICE_LINE_Z_NEAR = 1400.0f;
	constexpr float SERVICE_LINE_Z_FAR  = 6550.0f;

	/** ネットとセンターライン */
	constexpr float NET_Z         = 4000.0f;
	constexpr float CENTER_LINE_X = -100.0f;

	/** 地面からネットの高さ */
	constexpr float NET_HEIGHT = 250.0f;
}

