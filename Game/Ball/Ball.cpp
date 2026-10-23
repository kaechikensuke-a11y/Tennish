#include "stdafx.h"
#include "Ball.h"
#include "Source/Court/CourtConst.h"

namespace
{
	/** ボールのファイルパス */
	const char* BALL_MODEL_PATH = "Assets/model/Ball/Ball.tkm";

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
		m_ballSpeed.y += CourtConst::GRAVITY * m_gravityScale * dt;
		m_ballSpeed.x += m_curveAccelX * dt;

		/** 位置更新の前のZ座標を覚えておく */
		float prevZ = m_ballPosition.z;

		/** 位置更新を行う */
		m_ballPosition += m_ballSpeed * dt;

		/** ネットをまたいだか調べる(ネットにかかった後は調べない) */
		if (!m_isHitNet)
		{
			bool crossed = (prevZ - CourtConst::NET_Z) * (m_ballPosition.z - CourtConst::NET_Z) < 0.0f;
			if (crossed)
			{
				if (m_ballPosition.y < CourtConst::GROUND_Y + CourtConst::NET_HEIGHT)
				{
					/** ネットより低かったらその場で落とす */
					m_isHitNet = true;
					m_ballPosition.z = prevZ;
					m_ballSpeed.z = 0.0f;
					m_ballSpeed.x = 0.0f;
				}
				else
				{
					m_isCrossedNet = true;
				}
			}
		}

		/** 地面でバウンドさせる */
		if (m_ballPosition.y < CourtConst::GROUND_Y)
		{
			m_ballPosition.y = CourtConst::GROUND_Y;

			m_ballSpeed.y *= -BOUNCE * m_bounceScale;
			m_ballSpeed.x *= m_bounceScale;
			m_ballSpeed.z *= m_bounceScale;

			/** 着地したらショットの効果は終わり */
			m_curveAccelX = 0.0f;
			m_gravityScale = 1.0f;
			m_bounceScale = 1.0f;

			/** 1バウンド目だけ座標を記録する */
			if (m_bounceCount == 0)
			{
				m_firstBouncePos = m_ballPosition;
				m_isHasBounced = true;
			}
			m_bounceCount++;
		}
	}

	m_ballRender.SetPosition(m_ballPosition);
	m_ballRender.Update();
}

Vector3 Ball::PredictLanding() const
{
	float g = CourtConst::GRAVITY * m_gravityScale;
	float h = m_ballPosition.y - CourtConst::GROUND_Y;
	if (h < 0.0f) h = 0.0f;

	float disc = m_ballSpeed.y * m_ballSpeed.y - 2.0f * g * h;
	float t = (-m_ballSpeed.y - sqrtf(disc)) / g;

	/** 水平は等速 + カーブの加速度分 */
	return Vector3(m_ballPosition.x + m_ballSpeed.x * t + 0.5f * m_curveAccelX * t * t,
		CourtConst::GROUND_Y,
		m_ballPosition.z + m_ballSpeed.z * t);

}

void Ball::Render(RenderContext& rc)
{
	m_ballRender.Draw(rc);
}
