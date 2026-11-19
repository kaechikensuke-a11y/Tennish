#include "stdafx.h"
#include "CharacterAnimator.h"

namespace
{
	/** アニメーションのファイルパス */
	const char* ANIMATION_PATH[CharacterAnimator::en_ClipNum] =
	{
		"Assets/AnimData/IdleAnimation.tka",
		"Assets/AnimData/ServeToss.tka",
		"Assets/AnimData/ServeHit.tka",
		"Assets/AnimData/RunLeft.tka",
		"Assets/AnimData/RunRight.tka",
		"Assets/AnimData/ChargeLeft.tka",
		"Assets/AnimData/ChargeRight.tka",
		"Assets/AnimData/SwingLeft.tka",
		"Assets/AnimData/test.tka",
	};

	/** ループするかどうか */
	constexpr bool CLIP_LOOP[CharacterAnimator::en_ClipNum] =
	{
		true,
		false,
		false,
		true,
		true,
		true,
		true,
		false,
		false,
	};

	/** モーションブレンドの補完時間 */
	constexpr float BLEND_LOOP = 0.25f;
	constexpr float BLEND_SHOT = 0.15f;
}

void CharacterAnimator::Load()
{
	for (int i = 0; i < en_ClipNum; i++)
	{
		m_AnimationClips[i].Load(ANIMATION_PATH[i]);
		/** アニメーションをループするかどうか */
		m_AnimationClips[i].SetLoopFlag(CLIP_LOOP[i]);
	}
}

/** アニメーションがループする場合 */
void CharacterAnimator::PlayLoop(EnClip clip)
{
	/** 早期リターン */
	if (m_model   == nullptr)              return;
	if (m_current == clip && !m_isOneShot) return;

	m_current   = clip;
	m_isOneShot = false;
	/** キャラクターにアニメーションを再生させる */
	m_model->PlayAnimation(clip, BLEND_LOOP);
}

/** アニメーションが一回のみ再生される場合 */
void CharacterAnimator::PlayOneShot(EnClip clip)
{
	if (m_model == nullptr) return;

	m_current   = clip;
	m_isOneShot = true;
	m_model->PlayAnimation(clip, BLEND_SHOT);
}

/** 一回のみ再生されているアニメーションが再生されているかどうか確認する */
bool CharacterAnimator::IsOneShotPlaying() const
{
	if (m_model == nullptr || !m_isOneShot) return false;
	return m_model->IsPlayingAnimation();
}

void CharacterAnimator::Reset()
{
	m_isOneShot = false;
	m_current = en_ClipNum;
}
