#include "stdafx.h"
#include "Character.h"

namespace
{
	const char* PLAYER_MODEL_PATH = "Assets/model/Player/TennisPlayerBlue.tkm";
}

void Character::Update()
{
	/** どう動きたいかを子クラスから受け取る */
	Intent intent = DecideIntent();

	float dt = g_gameTime->GetFrameDeltaTime();

	m_X += intent.moveX * m_speed * dt;
	m_Z += intent.moveZ * m_speed * dt;
}