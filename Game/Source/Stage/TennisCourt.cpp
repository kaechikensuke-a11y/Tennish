#include "stdafx.h"
#include "TennisCourt.h"
namespace
{
	/** 通常ステージのファイルパス */
	const char* NORMAL_TENNIS_FILEPATH = "Assets/model/Stage/TennisCourt.tkm";

	/** 審判台のファイルパス */
	const char* UMPIRE_CHAIR_FILE_PATH = "Assets/model/TennisUmpireChair/TennisUmpireChair.tkm";

	/** ネットのファイルパス */
	const char* NET_FILE_PATH = "Assets/model/Stage/TennisNet.tkm";

	/** ステージの座標 */
	Vector3 STAGE_POS = { 0.0f,0.0f,0.0f };

	/** 審判台のファイルパス */
	Vector3 UMPIRECHAIR_POS = { -3000.0f,-900.0f,3700.0f };

	/** ネットのファイルパス */
	Vector3 NET_POS = { 0.0f,-900.0f,3750.0f };

}

bool TennisCourt::Start()
{
	/** 通常ステージのモデル */
	m_normalTennisCourt.Init(NORMAL_TENNIS_FILEPATH);
	m_normalTennisCourt.SetPosition(STAGE_POS);
	m_normalTennisCourt.Update();

	/** 審判台のモデル */
	m_UmpireChair.Init(UMPIRE_CHAIR_FILE_PATH);
	m_UmpireChair.SetPosition(UMPIRECHAIR_POS);
	m_UmpireChair.Update();

	m_TennisNet.Init(NET_FILE_PATH);
	m_TennisNet.SetPosition(NET_POS);
	m_TennisNet.Update();

	return true;
}

void TennisCourt::Update()
{

}

void TennisCourt::Render(RenderContext& rc)
{
	m_normalTennisCourt.Draw(rc);
	m_UmpireChair.Draw(rc);
	m_TennisNet.Draw(rc);
}