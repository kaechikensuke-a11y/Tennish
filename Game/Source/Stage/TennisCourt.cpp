#include "stdafx.h"
#include "TennisCourt.h"
namespace
{
	/** 通常ステージのファイルパス */
	const char* NORMAL_TENNIS_FILEPATH = "Assets/model/Stage/TennisCourt.tkm";

	/** ステージの座標 */
	Vector3 STAGE_POS = { 0.0f,0.0f,0.0f };

}

bool TennisCourt::Start()
{
	/** 通常ステージのモデル */
	m_normalTennisCourt.Init(NORMAL_TENNIS_FILEPATH);
	m_normalTennisCourt.SetPosition(STAGE_POS);
	m_normalTennisCourt.Update();

	return true;
}

void TennisCourt::Update()
{

}

void TennisCourt::Render(RenderContext& rc)
{
	m_normalTennisCourt.Draw(rc);
}