#include "stdafx.h"
#include "PauseState.h"

/** static変数の実体。最初は「ポーズしていない」状態 */
bool PauseState::m_paused = false;
int  PauseState::m_resumeFrames = 0;

bool PauseState::IsPaused()
{
	return m_paused || m_resumeFrames > 0;
}

void PauseState::Pause()
{
	m_paused = true;
	m_resumeFrames = 0;
}

void PauseState::Resume()
{
	m_paused = false;
	m_resumeFrames = RESUME_WAIT_FRAMES;
}

void PauseState::Reset()
{
	m_paused = false;
	m_resumeFrames = 0;
}

void PauseState::Tick()
{
	if (m_resumeFrames > 0)
	{
		m_resumeFrames--;
	}
}