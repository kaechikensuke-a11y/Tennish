#include "stdafx.h"
#include "Player.h"

Player::Intent Player::DecideIntent()
{
	Intent intent;

	/** 左スティックで移動 */
	intent.moveX = g_pad[0]->GetLStickXF();/** 左右 */
	intent.moveZ = g_pad[0]->GetLStickYF();/** 奥・手前 */

	/** スティックの左右で狙いの左右、奥に倒すほど深く打つ */
	intent.aimX = intent.moveX;

	/** RTボタンでトス */
	intent.isSwing = g_pad[0]->IsTrigger(enButtonRB2);

	return intent;
}