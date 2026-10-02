#pragma once
#include "Source/Character/Character.h"
class Player : public Character
{
protected:
	/** 親の仮想関数を実装 */
	Intent DecideIntent() override;
};

