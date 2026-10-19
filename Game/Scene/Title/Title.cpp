#include "stdafx.h"
#include "Title.h"
#include "SoundManager/SoundManager.h"
#include "SoundManager/BGM/BGM.h"
namespace
{
	/** タイトル画面のファイルパス */
	const char* TITLE_FILE_PATH = "Assets/Scene/Title/Title.dds";

	/** タイトル画面の大きさ */
	constexpr float WIDTH  = 1920.0f;
	constexpr float HEIGHT = 1080.0f;
}

bool Title::Start()
{
	m_titleRender.Init(TITLE_FILE_PATH, WIDTH, HEIGHT);


	return true;
}

void Render(RenderContext& rc)
{
	m_titleRender(rc);
}