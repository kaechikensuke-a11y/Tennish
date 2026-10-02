#include "stdafx.h"
#include "Character.h"

namespace
{
	const char* PLAYER_MODEL_PATH = "Assets/model/Player/TennisPlayerBlue.tkm";
	constexpr float PLAYER_POSITION_Y = -900.0f;
}

bool Character::Start()
{
	/** モデルの読み込み */
	m_modelRender.Init(PLAYER_MODEL_PATH);

	/** 初期位置をモデルに反映 */
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	return true;
}

void Character::Update()
{
	/** どう動きたいかを子クラスから受け取る */
	Intent intent = DecideIntent();

	/** 経過時間 */
	float dt = g_gameTime->GetFrameDeltaTime();

	/** 座標を動かす */
	m_position.x += intent.moveX * m_speed * dt;
	m_position.y = PLAYER_POSITION_Y;
	m_position.z += intent.moveZ * m_speed * dt;

	if (fabsf(intent.moveX) > 0.001f || fabsf(intent.moveZ) > 0.001f)
	{
		Vector3 dir(intent.moveX, 0.0f, intent.moveZ);
		m_rotation.SetRotationYFromDirectionXZ(dir);
	}
	/** 動かした座標をモデルに反映する */
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();
}

void Character::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}