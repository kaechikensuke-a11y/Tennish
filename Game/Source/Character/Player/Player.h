#pragma once
#include "Source/Character/Character.h"
class Player : public Character
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);
protected:
	Intent DecideIntent() override;
};

