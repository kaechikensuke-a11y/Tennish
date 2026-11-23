#include "stdafx.h"
#include "PauseMenu.h"
#include "SoundManager/SoundManager.h"

namespace
{
	const char* BLACK_PATH = "Assets/Scene/Fade/Blackout.dds";
	const char* BUTTON_PATH[3] =
	{
		"Assets/Scene/Pause/BackUI.dds",
		"Assets/Scene/Pause/OptionUI.dds",
		"Assets/Scene/Pause/FinishUI.dds"
	};

	constexpr float BLACK_W = 1920.0f;
	constexpr float BLACK_H = 1080.0f;
	constexpr float BUTTON_W = 500.0f;
	constexpr float BUTTON_H = 120.0f;

	/** 項目のY座標(上から順) */
	const Vector3 BUTTON_POS[3] =
	{
		Vector3(0.0f,  240.0f, 0.0f),
		Vector3(0.0f,    0.0f, 0.0f),
		Vector3(0.0f, -240.0f, 0.0f),
	};

	constexpr float BLACK_MAX_ALPHA = 0.6f;
	constexpr float FADE_TIME = 0.3f;
	constexpr float STICK_TILT = 0.5f;

	/** 0〜1に収めて、最初に速く最後にゆっくりにする */
	float EaseOut(float t)
	{
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f;
		return 1.0f - (1.0f - t) * (1.0f - t);
	}
}

bool PauseMenu::Start()
{
	m_blackOut.Init(BLACK_PATH, BLACK_W, BLACK_H);
	m_blackOut.SetPosition(Vector3::Zero);
	m_blackOut.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	m_blackOut.Update();

	for (int i = 0; i < en_ItemNum; i++)
	{
		m_button[i].Init(BUTTON_PATH[i], BUTTON_W, BUTTON_H);
		m_button[i].SetPosition(BUTTON_POS[i]);
		m_button[i].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
		m_button[i].Update();
	}
	return true;
}

void PauseMenu::Update()
{
	m_timer += g_gameTime->GetFrameDeltaTime();
	const float t = EaseOut(m_timer / FADE_TIME);

	/** 暗幕 */
	m_blackOut.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, BLACK_MAX_ALPHA * t));
	m_blackOut.Update();

	/** 項目: 選択中は大きく明るく、他は暗く */
	for (int i = 0; i < en_ItemNum; i++)
	{
		const bool sel = (i == m_index);
		const float s = (0.8f + 0.2f * t) * (sel ? 1.1f : 1.0f);
		const float c = sel ? 1.0f : 0.6f;
		m_button[i].SetScale(Vector3(s, s, 1.0f));
		m_button[i].SetMulColor(Vector4(c, c, c, t));
		m_button[i].Update();
	}

	/** 開いた瞬間の入力は無視する */
	if (m_firstFrame)
	{
		m_firstFrame = false;
		return;
	}

	/** 決定後は入力を受け付けない */
	if (m_decided != -1) return;

	bool up = g_pad[0]->IsTrigger(enButtonUp);
	bool down = g_pad[0]->IsTrigger(enButtonDown);

	/** スティック上下。倒したままでも1回だけ反応させる */
	const float stickY = g_pad[0]->GetLStickYF();
	if (fabsf(stickY) < STICK_TILT)
	{
		m_stickReleased = true;
	}
	else if (m_stickReleased)
	{
		if (stickY > 0.0f) up = true;
		else               down = true;
		m_stickReleased = false;
	}

	const int old = m_index;
	if (down) m_index = (m_index + 1) % en_ItemNum;
	if (up)   m_index = (m_index + en_ItemNum - 1) % en_ItemNum;

	if (m_index != old)
	{
		SoundManager::GetInstance()->PlaySE(SEType::en_SelectSE);
	}

	/** Aで決定 */
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		SoundManager::GetInstance()->PlaySE(SEType::en_DecideSE);
		m_decided = m_index;
	}
}

void PauseMenu::Render(RenderContext& rc)
{
	m_blackOut.Draw(rc);
	for (int i = 0; i < en_ItemNum; i++)
	{
		m_button[i].Draw(rc);
	}
}