#include "stdafx.h"
#include "FadeScreen.h"

namespace
{
	/** 白い画面で覆う画像のファイルパスと大きさ */
	const char* FADE_PATH = "Assets/Scene/Fade/Whiteout.dds";
	constexpr float FADE_W = 1920.0f;
	constexpr float FADE_H = 1080.0f;
}

bool FadeScreen::Start()
{
	m_sprite.Init(FADE_PATH, FADE_W, FADE_H);
	m_sprite.SetPosition(Vector3::Zero);
	m_sprite.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	m_sprite.Update();
	return true;
}

void FadeScreen::StartFadeOut(float seconds)
{
	m_state = State::en_FadeOut;
	m_duration = seconds;
	m_timer = 0.0f;
}

void FadeScreen::StartFadeIn(float seconds)
{
	m_state = State::en_FadeIn;
	m_duration = seconds;
	m_timer = 0.0f;
}

void FadeScreen::Update()
{
	if (m_state == State::en_Idle) return;

	m_timer += g_gameTime->GetFrameDeltaTime();
	float t = m_timer / m_duration;
	if (t > 1.0f) t = 1.0f;

	if (m_state == State::en_FadeOut)
	{
		m_alpha = t;
		if (t >= 1.0f) m_state = State::en_Idle; /** 白のまま止まる */
	}
	else
	{
		m_alpha = 1.0f - t;
		if (t >= 1.0f) m_state = State::en_Idle; /** 透明に戻って止まる */
	}

	m_sprite.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
	m_sprite.Update();
}

void FadeScreen::Render(RenderContext& rc)
{
	/** 完全に透明なら描かない */
	if (m_alpha <= 0.0f) return;
	m_sprite.Draw(rc);
}