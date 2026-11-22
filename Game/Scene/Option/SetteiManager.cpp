#include "stdafx.h"
#include "SetteiManager.h"

namespace
{
	/** 範囲に収める */
	int ClampLevel(int level)
	{
		if (level < 0) return 0;
		if (level > SetteiManager::VOLUME_MAX) return SetteiManager::VOLUME_MAX;
		return level;
	}
}

SetteiManager* SetteiManager::m_instance = nullptr;

bool SetteiManager::Start()
{
	m_instance = this;
	return true;
}

void SetteiManager::SetBGMLevel(int level)
{
	/** クラス外から値が変更されても保存するようにする */
	m_bgmLevel = ClampLevel(level);
}

void SetteiManager::SetSELevel(int level)
{
	m_seLevel = ClampLevel(level);
}