#include "stdafx.h"
#include "Ball.h"

namespace
{
	/** ボールのファイルパス */
	const char* BALL_MODEL_PATH = "Assets/model/Ball/Ball.tkm";

	/** 重力 */
	constexpr float GRAVITY = -1200.0f;
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

		/** 重力とカーブの */
		m_ballSpeed.y += GRAVITY * m_gravityScale * dt;
		m_ballSpeed.x += m_curveAccelX * dt;

		/** 位置更新を行う */
		m_ballPosition += m_ballSpeed * dt;

		/** 地面でバウンドさせる */
		if (m_ballPosition.y < GROUND_Y)
		{
			m_ballPosition.y = GROUND_Y;

			m_ballSpeed.y *= -BOUNCE * m_bounceScale;
			m_ballSpeed.x *= m_bounceScale;
			m_ballSpeed.z *= m_bounceScale;

			/** 着地したらショットの効果は終わり */
			m_curveAccelX = 0.0f;
			m_gravityScale = 1.0f;
			m_bounceScale = 1.0f;

			m_bounceCount++;
		}
	}

	m_ballRender.SetPosition(m_ballPosition);
	m_ballRender.Update();
}

Vector3 Ball::PredictLanding() const
{
	float g = GRAVITY * m_gravityScale;
	float h = m_ballPosition.y - GROUND_Y;
	if (h < 0.0f) h = 0.0f;

	float disc = m_ballSpeed.y * m_ballSpeed.y - 2.0f * g * h;
	float t = (-m_ballSpeed.y - sqrtf(disc)) / g;

	/** 水平は等速 + カーブの加速度分 */
	return Vector3(m_ballPosition.x + m_ballSpeed.x * t + 0.5f * m_curveAccelX * t * t,
		GROUND_Y,
		m_ballPosition.z + m_ballSpeed.z * t);

}

void Ball::Render(RenderContext& rc)
{
	m_ballRender.Draw(rc);
}
