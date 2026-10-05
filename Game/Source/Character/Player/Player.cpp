#include "stdafx.h"
#include "Player.h"

Player::Intent Player::DecideIntent()
{
	Intent intent;

	/** 左スティックで移動 */
	intent.moveX = g_pad[0]->GetLStickXF();
	intent.moveZ = g_pad[0]->GetLStickYF();

	/** RTボタンでトス */
	intent.isSwing = g_pad[0]->IsTrigger(enButtonRB2);

	return intent;
}