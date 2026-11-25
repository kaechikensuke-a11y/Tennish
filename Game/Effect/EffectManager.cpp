#include "stdafx.h"
#include "EffectManager.h"

EffectManager* EffectManager::m_instance = nullptr;

bool EffectManager::Start()
{
	m_instance = this;

	/** エフェクトを登録する */
	EffectEngine::GetInstance()->ResistEffect
	(
		(int)EffectType::en_ChargeFirst,
		u"Assets/Effect/ChargeFarst/ChargeFirst.efk"
	);
	EffectEngine::GetInstance()->ResistEffect
	(
		(int)EffectType::en_ChargeSecond,
		u"Assets/Effect/ChargeFarst/ChargeSecond.efk"
	);
	EffectEngine::GetInstance()->ResistEffect
	(
		(int)EffectType::en_ChargeThird,
		u"Assets/Effect/ChargeFarst/ChargeThird.efk"
	);

	return true;
}

int EffectManager::Play(EffectType type, const Vector3& pos, float scale)
{
	EffectEngine* engine = EffectEngine::GetInstance();

	/** 登録したエフェクトを取り出して再生する */
	Effekseer::EffectRef effect = engine->LoadEffect((int)type);
	const int handle = engine->Play(effect);
	if (handle < 0)
	{
		/** 呼び出しに失敗した時用に-1を返す */
		return -1;
	}

	/** 場所と大きさを決める */
	Move(handle, pos, scale);
	return handle;
}

void EffectManager::Move(int handle, const Vector3& pos, float scale)
{
	if (handle < 0)
	{
		return;
	}

	EffectEngine::GetInstance()->UpdateEffectWorldMatrix
	(
		handle, pos, g_quatIdentity, Vector3(scale, scale, scale)
	);
}

void EffectManager::Stop(int handle)
{
	if (handle < 0)
	{
		return;
	}

	EffectEngine::GetInstance()->Stop(handle);
}