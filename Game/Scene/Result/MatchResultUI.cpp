#include "stdafx.h"
#include <cmath>
#include "MatchResultUI.h"

namespace
{
	/** 終了画像 */
	const char* END_IMAGE_PATH     = "Assets/Scene/Result/GameFin.dds";
	const char* ENDBACK_IMAGE_PATH = "Assets/Scene/Result/GameFinBack.dds";
	const char* BANDBACK_PATH = "Assets/Scene/Result/WinBackAnimation.dds";
	const char* BAND_PATH = "Assets/Scene/Result/WinBackGround.dds";
	const char* WIN_PATH  = "Assets/Scene/Result/WinText.dds";

	constexpr float END_IMAGE_W = 800.0f;
	constexpr float END_IMAGE_H = 250.0f;

	constexpr float ENDBACK_IMAGE_W = 1000.0f;
	constexpr float ENDBACK_IMAGE_H = 400.0f;

	constexpr float BAND_W = 2600.0f;
	constexpr float BAND_H = 700.0f;

	constexpr float WIN_W = 520.0f;
	constexpr float WIN_H = 220.0f;

	constexpr float BANDBACK_W = 500.0f;
	constexpr float BANDBACK_H = 160.0f;

	/** 最終位置 */
	const Vector3 BAND_END_POS(-200.0f, 380.0f, 0.0f);
	const Vector3 WIN_END_POS(-440.0f, 330.0f, 0.0f);
	const Vector3 FLOW_END_POS(-900.0f, 110.0f, 0.0f);

	/** 開始位置 */
	const Vector3 SPRITE_START_POS(900.0f,900.0f,0.0f);
	const Vector3 BANDBACK_START_POS(400.0f,500.0f,0.0f);

	constexpr float POP_TIME      = 1.3f; /** 出現させる時間 */
	constexpr float HOLD_TIME     = 0.5f; /** 保持 */
	constexpr float POP_OVERSHOOT = 1.3f; /** 拡大させる大きさ */
	constexpr float POP_SPRIT     = 0.67; /** 出現のうち、拡大に当てる割合 */

	constexpr float BAND_TIME   = 0.5f;  /** 帯が流れ込む時間 */
	constexpr float WIN_DELAY   = 0.15f; /** 帯から文字が遅れる時間 */
	constexpr float WIN_TIME    = 0.4f;	 /** 文字が流れ込む時間 */
	constexpr float SCORE_DELAY = 0.7f;	 /** 得点が出始める時間 */
	constexpr float SCORE_TIME  = 0.3f;	 /** 得点のポップ時間 */

	constexpr float FLOW_TIME = 3.5f;     /** 1回の移動にかける時間 */     
	constexpr float FLOW_FADE_IN  = 0.6f; /** フェードインにかける時間 */
	constexpr float FLOW_FADE_OUT = 0.3f; /** フェードアウトにかける時間 */

	/** t(0〜1)から拡大率を返す: 0倍 → 1.3倍に膨らむ → 1.0倍に戻る */
	float CalcPopScale(float t)
	{
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f;

		/** 拡大していく */
		if (t < POP_SPRIT)
		{
			float u = t / POP_SPRIT;
			float ease = 1.0f - (1.0f - u) * (1.0f - u);
			return POP_OVERSHOOT * ease;
		}
		
		/** 縮小していく */
		float u = (t - POP_SPRIT) / (1.0f - POP_SPRIT);
		return POP_OVERSHOOT + (1.0f - POP_OVERSHOOT) * u;
	}

	/** 減速させる */
	float EaseOut(float t)
	{
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f;
		return 1.0f - (1.0f - t) * (1.0f - t);
	}

	Vector3 Lerp3(const Vector3& a, const Vector3& b, float t)
	{
		return Vector3(a.x + (b.x - a.x) * t,
					   a.y + (b.y - a.y) * t,
					   a.z + (b.z - a.z) * t);
	}

	float CalcFlowAlpha(float time)
	{
		/** 移動前、移動後は非表示 */
		if (time <= 0.0f || time >= FLOW_TIME) return 0.0f;

		/** フェードイン */
		if (time < FLOW_FADE_IN)
		{
			return time / FLOW_FADE_IN;
		}

		/** フェードアウト */
		float outStart = FLOW_TIME - FLOW_FADE_OUT;
		if (time > outStart)
		{
			return (FLOW_TIME - time) / FLOW_FADE_OUT;
		}

		return 1.0f;
	}
	
}

bool MatchResultUI::Start()
{
	m_endImage.Init(END_IMAGE_PATH, END_IMAGE_W, END_IMAGE_H);
	m_endImage.SetPosition(Vector3::Zero);
	m_endImage.Update();

	m_endBackImage.Init(ENDBACK_IMAGE_PATH, ENDBACK_IMAGE_W, ENDBACK_IMAGE_H);
	m_endBackImage.SetPosition(Vector3::Zero);
	m_endBackImage.Update();

	m_band.Init(BAND_PATH, BAND_W, BAND_H);
	m_band.SetPosition(SPRITE_START_POS);
	m_band.Update();

	m_winText.Init(WIN_PATH, WIN_W, WIN_H);
	m_winText.SetPosition(SPRITE_START_POS);
	m_winText.Update();


	m_flow.Init(BANDBACK_PATH, BANDBACK_W, BANDBACK_H);
	m_flow.SetPosition(BANDBACK_START_POS);
	m_flow.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	m_flow.Update();

	return true;
}

void MatchResultUI::ShowEndImage()
{
	m_phase = Phase::en_Appear;
	m_timer = 0.0f;
	m_scale = 0.0f;
}

void MatchResultUI::ShowResult(int playerGames, int enemyGames)
{
	m_resultActive = true;
	m_resultTimer = 0.0f;
}

void MatchResultUI::Update()
{
	if (m_resultActive)
	{
		m_resultTimer += g_gameTime->GetFrameDeltaTime();

		/** 帯: 右上から左上の最終位置へ */
		float tb = EaseOut(m_resultTimer / BAND_TIME);
		m_band.SetPosition(Lerp3(SPRITE_START_POS, BAND_END_POS, tb));
		m_band.Update();

		/** 勝ち!: 帯から少し遅れて、同じ方向から */
		float tw = EaseOut((m_resultTimer - WIN_DELAY) / WIN_TIME);
		m_winText.SetPosition(Lerp3(SPRITE_START_POS, WIN_END_POS, tw));
		m_winText.Update();

		float flowTime = std::fmod(m_resultTimer, FLOW_TIME);
		float ts = flowTime / FLOW_TIME;
		m_flow.SetPosition(Lerp3(BANDBACK_START_POS,FLOW_END_POS , ts));
		m_flow.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, CalcFlowAlpha(flowTime)));
		m_flow.Update();
		
	}
	/*
	 * 試合終了のアニメーションのフロー
	 * 1. 1.2秒間拡大させる
	 * 2. 0.5秒間大きさを維持させる
	 * 3. 1.2秒間縮小させる
	 */
	if (m_phase == Phase::en_Hidden || m_phase == Phase::en_Done) return;

	m_timer += g_gameTime->GetFrameDeltaTime();

	switch (m_phase)
	{
	case Phase::en_Appear:
		m_scale = CalcPopScale(m_timer / POP_TIME);
		if (m_timer >= POP_TIME)
		{
			m_phase = Phase::en_Hold;
			m_timer = 0.0f;
		}
		break;

	case Phase::en_Hold:
		m_scale = 1.0f;
		if (m_timer >= HOLD_TIME)
		{
			m_phase = Phase::en_Disappear;
			m_timer = 0.0f;
		}
		break;

	case Phase::en_Disappear:
		m_scale = CalcPopScale(1.0f - m_timer / POP_TIME);
		if (m_timer >= POP_TIME)
		{
			m_phase = Phase::en_Done;
			m_scale = 0.0f;
		}
		break;

	default:
		break;
	}

	m_endImage.SetScale(Vector3(m_scale, m_scale, 1.0f));
	m_endImage.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.8f));
	m_endImage.Update();

	m_endBackImage.SetScale(Vector3(m_scale, m_scale, 1.0f));
	m_endBackImage.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.8f));
	m_endBackImage.Update();
}

void MatchResultUI::Render(RenderContext& rc)
{
	if (m_resultActive)
	{
		m_band.Draw(rc);
		m_flow.Draw(rc);
		m_winText.Draw(rc);
	}

	if (m_phase == Phase::en_Hidden || m_phase == Phase::en_Done) return;
	if (m_scale <= 0.001f) return;
	m_endBackImage.Draw(rc);
	m_endImage.Draw(rc);
	
}