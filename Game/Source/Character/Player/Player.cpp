#include "stdafx.h"
#include "Player.h"

namespace
{
	/** A/B/X/Y のどれかを押した瞬間なら、そのショットを返す */
	bool GetTriggerShot(ShotType& shot)
	{
		if (g_pad[0]->IsTrigger(enButtonB)) { shot = ShotType::en_Normal;   return true; }
		if (g_pad[0]->IsTrigger(enButtonA)) { shot = ShotType::en_BackSpin; return true; }
		if (g_pad[0]->IsTrigger(enButtonY)) { shot = ShotType::en_TopSpin;  return true; }
		if (g_pad[0]->IsTrigger(enButtonX)) { shot = ShotType::en_Curve;    return true; }
		return false;
	}

	/** そのショットのボタンを押し続けているか(離した瞬間に打ち返す) */
	bool IsPressShot(ShotType shot)
	{
		switch (shot)
		{
		case ShotType::en_Normal:   return g_pad[0]->IsPress(enButtonB);
		case ShotType::en_BackSpin: return g_pad[0]->IsPress(enButtonA);
		case ShotType::en_TopSpin:  return g_pad[0]->IsPress(enButtonY);
		case ShotType::en_Curve:    return g_pad[0]->IsPress(enButtonX);
		}
		return false;
	}
}

	Player::Intent Player::DecideIntent()
	{
		Intent intent;

		/** 左スティックで移動 */
		intent.moveX = g_pad[0]->GetLStickXF();
		intent.moveZ = g_pad[0]->GetLStickYF();
		intent.aimX = intent.moveX; /** 左右の狙い兼カーブの向き */

		ShotType trigger;

		/** サーブ中(トス・打つ)はチャージなし。押した瞬間に動く */
		/** サーブはどのB/A/Y/X度のボタンを押してもサーブが打てるようにする(どのボタンを押しても同じ動き) */
		if (m_serveState == ServeState::en_Ready || m_serveState == ServeState::en_Tossed)
		{
			m_isCharging = false;
			m_chargeTime = 0.0f;
			if (GetTriggerShot(trigger))
			{
				intent.isSwing = true;
			}
			return intent;
		}

		/** ラリー: 押している間はチャージ、離した瞬間に打つ */
		if (!m_isCharging)
		{
			/** どれかのボタンを押した瞬間にチャージ開始(最初のボタンを優先) */
			if (GetTriggerShot(trigger))
			{
				m_isCharging = true;
				m_chargeShot = trigger; /** どのボタンでチャージしているか記録 */
				m_chargeTime = 0.0f;
			}
		}
		else
		{
			/** チャージ中は押している時間を数える */
			m_chargeTime += g_gameTime->GetFrameDeltaTime();

			if (IsPressShot(m_chargeShot))
			{
				/** チャージ継続中: 移動を遅くするため段階を渡す */
				intent.isCharging = true;
				/** 0.5秒でLv.1  1.0秒でLv.2にする */
				intent.chargeLevel = CalcChargeLevel(m_chargeTime);
			}
			else
			{
				/** 離した: その段階で打つ */
				intent.isSwing = true;
				intent.shot = m_chargeShot;
				intent.chargeLevel = CalcChargeLevel(m_chargeTime);

				/** 打った後に初期化する */
				m_isCharging = false;
				m_chargeTime = 0.0f;
			}
		}

		return intent;
	}