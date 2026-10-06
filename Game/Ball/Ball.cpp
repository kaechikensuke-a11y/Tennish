#include "stdafx.h"
#include "Ball.h"

namespace
{
	/** ボールのファイルパス */
	const char* BALL_MODEL_PATH = "Assets/model/Ball/Ball.tkm";

	/** 重力 */
	constexpr float GRAVITY = -980.0f;
	/** 地面の高さ */
	constexpr float GROUND_Y = -680.0f;
	/** バウンドの強さ */
	constexpr float BOUNCE = 0.7f;
}

bool Ball::Start()
{
	m_ballRender.Init(BALL_MODEL_PATH);
	m_ballRender.SetPosition(m_ballPosition);
	m_ballRender.Update();
	return true;
}

void Ball::Update()
{
	/** ボールをプレイヤーが持っている間は物理計算を行わない */
	if (!m_isHeld)
	{
		float dt = g_gameTime->GetFrameDeltaTime();

		m_ballSpeed.y += GRAVITY * dt;
		m_ballPosition += m_ballSpeed * dt;

		/** 地面でバウンドさせる */
		if (m_ballPosition.y < GROUND_Y)
		{
			m_ballPosition.y = GROUND_Y;
			m_ballSpeed.y *= -BOUNCE;
			m_bounceCount++;
		}
	}

	m_ballRender.SetPosition(m_ballPosition);
	m_ballRender.Update();
}

void Ball::Render(RenderContext& rc)
{
	m_ballRender.Draw(rc);
}
