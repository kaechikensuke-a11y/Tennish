#include "stdafx.h"
#include "Player.h"

Player::Intent Player::DecideIntent()
{
	Intent intent;

	intent.moveX = g_pad[0]->GetLStickXF();
	intent.moveZ = g_pad[0]->GetLStickYF();

	return intent;
}