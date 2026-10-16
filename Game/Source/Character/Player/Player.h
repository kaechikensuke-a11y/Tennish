#pragma once
#include "Source/Character/Character.h"
class Player : public Character
{
protected:
	/** 親の仮想関数を実装 */
	Intent DecideIntent() override;

private:
	bool     m_isCharging = false;                 /** チャージ中か */
	float    m_chargeTime = 0.0f;                  /** 押している時間 */
	ShotType m_chargeShot = ShotType::en_Normal;   /** チャージしているショット */
};

