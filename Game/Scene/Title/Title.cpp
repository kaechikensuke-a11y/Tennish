#include "stdafx.h"
#include "Title.h"
#include "SoundManager/SoundManager.h"
#include "SoundManager/BGM/BGM.h"
#include "SoundManager/SE/SE.h"
#include "Game.h"
namespace
{
	/** タイトル画面のファイルパス */
	const char* TITLE_FILE_PATH  = "Assets/Scene/Title/Title.dds";
	const char* START_FILE_PATH  = "Assets/Scene/Title/StartBlack.dds";
	const char* SETTEI_FILE_PATH = "Assets/Scene/Title/SetteiBlack.dds";

	/** タイトル画面の大きさ */
	constexpr float WIDTH  = 1920.0f;
	constexpr float HEIGHT = 1080.0f;

	/** 開始画面の大きさ */
	constexpr float START_WIDTH  = 700.0f;
	constexpr float START_HEIGHT = 140.0f;

	/** 開始画面の座標 */
	constexpr float START_POS_X = -315.0f;
	constexpr float START_POS_Y = -45.0f;

	/** 設定画面の大きさ */
	constexpr float SETTEI_WIDTH  = 410.0f;
	constexpr float SETTEI_HEIGHT = 70.0f;

	/** 設定画面の座標 */
	constexpr float SETTEI_POS_X = -330.0f;
	constexpr float SETTEI_POS_Y =  -170.0f;

	/** スティックを倒したと判定する値 */
	constexpr float STICK_TILT = 0.5f;

	/** 画像の透明度 */
	constexpr float SHADE = 0.5f;
}

bool Title::Start()
{
	m_titleRender.Init(TITLE_FILE_PATH, WIDTH, HEIGHT);

	/** スタート画面の半透明画像 */
	m_startRender.Init(START_FILE_PATH, START_WIDTH, START_HEIGHT);
	m_startRender.SetPosition(Vector3(START_POS_X, START_POS_Y, 0.0f));
	m_startRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, SHADE));
	m_startRender.Update();

	/** 設定画面の半透明画像 */
	m_setteiRender.Init(SETTEI_FILE_PATH, SETTEI_WIDTH, SETTEI_HEIGHT);
	m_setteiRender.SetPosition(Vector3(SETTEI_POS_X, SETTEI_POS_Y, 0.0f));
	m_setteiRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, SHADE));
	m_setteiRender.Update();

	SoundManager::GetInstance()->PlayBGM(BGMType::en_TitleBGM);

	return true;
}

void Title::Update()
{
	/** 十字キー・スティックで選択を動かす */
	MoveSelect();

	/** Bボタンで決定 */
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		Decide();
	}
}

void Title::MoveSelect()
{
	/** 動かす前の選択を保存する */
	Menu oldSelect = m_select;

	/** 十字キー */
	bool up   = g_pad[0]->IsTrigger(enButtonUp);
	bool down = g_pad[0]->IsTrigger(enButtonDown);

	/** スティック入力 */
	float stickY = g_pad[0]->GetLStickYF();

	if (fabsf(stickY) < STICK_TILT)
	{
		m_stickReleased = true;
	}
	else if (m_stickReleased)
	{
		if (stickY > 0.0f) up   = true;
		else               down = true;
		m_stickReleased        = false;
	}

	/** 上で開始、下で設定 */
	if  (up) m_select = Menu::en_Start;
	if (down) m_select = Menu::en_Settei;

	/** 選択が変わったときだけSEを鳴らす */
	if (m_select != oldSelect)
	{
		SoundManager::GetInstance()->PlaySE(SEType::en_SelectSE);
	}
}

void Title::Decide()
{
	switch (m_select)
	{
	case Menu::en_Start:
		SoundManager::GetInstance()->PlaySE(SEType::en_DecideSE);
		SoundManager::GetInstance()->StopBGM();

		/** ゲームを開始してタイトル画面を消す */
		NewGO<Game>(0, "game");
		DeleteGO(this);
		break;

	case Menu::en_Settei:
		SoundManager::GetInstance()->PlaySE(SEType::en_DecideSE);
		/** 設定画面へ向かう */
		DeleteGO(this);
		break;
	default:
		break;
	}
}

void Title::Render(RenderContext& rc)
{
	m_titleRender.Draw(rc);

	if (m_select != Menu::en_Start)
	{
		m_startRender.Draw(rc);
	}

	if (m_select != Menu::en_Settei)
	{
		m_setteiRender.Draw(rc);
	}
}