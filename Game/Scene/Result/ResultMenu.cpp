#include "stdafx.h"
#include "ResultMenu.h"

namespace
{
	const char* BLACK_PATH = "Assets/Scene/Fade/Blackout.dds";
	const char* BUTTON_PATH[2] =
	{
		"Assets/Scene/Result/RetryFont.dds",
		"Assets/Scene/Result/TitleFont.dds"
	};

	constexpr float BLACK_W = 1920.0f;
	constexpr float BLACK_H = 1080.0f;
	constexpr float BUTTON_W = 500.0f;
	constexpr float BUTTON_H = 120.0f;

	const Vector3 BUTTON_POS[2] =
	{
		Vector3(0.0f,60.0f,0.0f),
		Vector3(0.0f,-100.0f,0.0f),
	};

	constexpr float BLACK_MAX_ALPHA = 0.6f;
	constexpr float FADE_TIME = 0.3f;

	float EaseOut(float t)
	{
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f;
		return 1.0f - (1.0f - t) * (1.0f - t);
	}

}

bool ResultMenu::Start()
{
	m_blackOut.Init(BLACK_PATH, BLACK_W, BLACK_H);
	m_blackOut.SetPosition(Vector3::Zero);
	m_blackOut.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	m_blackOut.Update();

	for (int i = 0; i < MENU_NUM; i++)
	{
		m_button[i].Init(BUTTON_PATH[i], BUTTON_W, BUTTON_H);
		m_button[i].SetPosition(BUTTON_POS[i]);
		m_button[i].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
		m_button[i].Update();
	}
	return true;
}

void ResultMenu::Update()
{
	m_timer += g_gameTime->GetFrameDeltaTime();
	float t = EaseOut(m_timer / FADE_TIME);

	/** 暗幕 */
	m_blackOut.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, BLACK_MAX_ALPHA * t));
	m_blackOut.Update();

	/** ボタン: 選択中は大きく明るく */
	for (int i = 0; i < MENU_NUM; i++)
	{
		const bool sel = (i == m_index);
		float s = (0.8f + 0.2f * t) * (sel ? 1.1f : 1.0f);
		float c = sel ? 1.0f : 0.6f;
		m_button[i].SetScale(Vector3(s, s, 1.0f));
		m_button[i].SetMulColor(Vector4(c, c, c, t));
		m_button[i].Update();
	}

	/** 開いた瞬間のAは無視する */
	if (m_firstFrame)
	{
		m_firstFrame = false;
		return;
	}

	/** 決定後は入力を受け付けない */
	if (m_decided != -1) return;

	if (g_pad[0]->IsTrigger(enButtonDown)) m_index = (m_index + 1) % MENU_NUM;
	if (g_pad[0]->IsTrigger(enButtonUp))   m_index = (m_index + MENU_NUM - 1) % MENU_NUM;

	if (g_pad[0]->IsTrigger(enButtonA)) m_decided = m_index;
	if (g_pad[0]->IsTrigger(enButtonB)) m_closed = true;
}

void ResultMenu::Render(RenderContext& rc)
{
	m_blackOut.Draw(rc);
	for (int i = 0; i < MENU_NUM; i++) m_button[i].Draw(rc);
}