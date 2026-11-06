#include "stdafx.h"
#include "MatchEndDirector.h"
#include "MatchResultUI.h"
#include "Source/Character/Character.h"

void MatchEndDirector::Start(Character* player, Character* enemy)
{
	/** 操作を止める */
	player->SetControlEnabled(false);
	enemy->SetControlEnabled(false);

	/** 終了画像を出す */
	m_resultUI = NewGO<MatchResultUI>(5, "matchresultui");
	m_resultUI->ShowEndImage();
}

void MatchEndDirector::Update()
{
	/** 終了画像が消えたら */
}

void MatchEndDirector::Finish()
{
	if (m_resultUI != nullptr)
	{
		DeleteGO(m_resultUI);
		m_resultUI = nullptr;
	}
}