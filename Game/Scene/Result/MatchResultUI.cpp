#include "stdafx.h"
#include "MatchResultUI.h"

namespace
{
	/** 終了画像 */
	const char* END_IMAGE_PATH     = "Assets/Scene/Result/GameFin.dds";
	const char* ENDBACK_IMAGE_PATH = "Assets/Scene/Result/GameFinBack.dds";

	constexpr float END_IMAGE_W = 800.0f;
	constexpr float END_IMAGE_H = 250.0f;

	constexpr float ENDBACK_IMAGE_W = 1000.0f;
	constexpr float ENDBACK_IMAGE_H = 400.0f;

	constexpr float POP_TIME      = 1.3f; /** 出現させる時間 */
	constexpr float HOLD_TIME     = 0.5f; /** 保持 */
	constexpr float POP_OVERSHOOT = 1.3f; /** 拡大させる大きさ */
	constexpr float POP_SPRIT     = 0.67; /** 出現のうち、拡大に当てる割合 */

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
	
}

bool MatchResultUI::Start()
{
	m_endImage.Init(END_IMAGE_PATH, END_IMAGE_W, END_IMAGE_H);
	m_endImage.SetPosition(Vector3::Zero);
	m_endImage.Update();

	m_endBackImage.Init(ENDBACK_IMAGE_PATH, ENDBACK_IMAGE_W, ENDBACK_IMAGE_H);
	m_endBackImage.SetPosition(Vector3::Zero);
	m_endBackImage.Update();

	return true;
}

void MatchResultUI::ShowEndImage()
{
	m_phase = Phase::en_Appear;
	m_timer = 0.0f;
	m_scale = 0.0f;
}

void MatchResultUI::Update()
{
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
	if (m_phase == Phase::en_Hidden || m_phase == Phase::en_Done) return;
	if (m_scale <= 0.001f) return;
	m_endBackImage.Draw(rc);
	m_endImage.Draw(rc);
	
}