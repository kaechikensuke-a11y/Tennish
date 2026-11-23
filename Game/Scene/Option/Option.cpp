#include "stdafx.h"
#include "Option.h"
#include "SetteiManager.h"
#include "SoundManager/SoundManager.h"

namespace
{
	/** 背景画像のファイルパス */
	const char* BACKGROUND_PATH = "Assets/Scene/Option/SetteiScene.dds";

	/** 棒のファイルパス */
	const char* BAR_PATH = "Assets/Scene/Option/GreenBar.dds";
	const char* BLACK_BAR_PATH = "Assets/Scene/Option/Bar.dds";

	/** 文字のファイルパス */
	const char* SE_PATH = "Assets/Scene/Option/SEFont.dds";
	const char* BGM_PATH = "Assets/Scene/Option/BGMFont.dds";
	const char* VIBRATION_PATH = "Assets/Scene/Option/vibrationFont.dds";
	const char* COMEBACK_PATH = "Assets/Scene/Option/ComeBack.dds";
	const char* COMEBACKBLACK_PATH = "Assets/Scene/Option/ComeBackBlack.dds";

	/** バイブレーションのファイルパス */
	const char* ONSWITCH_PATH  = "Assets/Scene/Option/ONSwitch.dds";
	const char* OFFSWITCH_PATH = "Assets/Scene/Option/OFFSwitch.dds";

	/** 背景画像の大きさ */
	constexpr float BACKGROUND_W = 1920.0f;
	constexpr float BACKGROUND_H = 1080.0f;

	/** 棒の大きさ */
	constexpr float BAR_W = 55.0f;
	constexpr float BAR_H = 90.0f;

	/** 文字の大きさ */
	constexpr float FONT_W = 65.0f;
	constexpr float FONT_H = 110.0f;
	constexpr float COMEBACK_W = 390.0f;
	constexpr float COMEBACK_H = 130.0f;

	/** バイブレーション画像の大きさ */
	constexpr float SWITCH_W = 300.0f;
	constexpr float SWITCH_H = 130.0f;

	/** 棒の横並びを等間隔で開ける */
	constexpr float BAR_START_X = -150.0f;
	constexpr float BAR_SPACING =   85.0f;

	/** 画像のポジション */
	constexpr float ROW_BGM_Y = 260.0f;
	constexpr float ROW_SE_Y = 90.0f;
	constexpr float ROW_VIBRATION_Y = -80.0f;
	constexpr float ROW_COMEBACK_Y = -300.0f;

	/** 音量の行ごとのY座標を、番号で取り出せるようにする */
	constexpr float VOLUME_ROW_Y[2] = { ROW_BGM_Y, ROW_SE_Y };

	/** 項目名の画像のX座標 */
	constexpr float LABEL_X = -560.0f;

	/** バイブレーションのスイッチのX座標 */
	constexpr float SWITCH_X = 200.0f;

	/** 戻る画像のX座標 */
	constexpr float COMEBACK_X = 0.0f;

	/** スティックを倒したと判定する値 */
	constexpr float STICK_TILT = 0.5f;

	/** 選択中の拡大率、選択していないときの明るさ */
	constexpr float SELECT_SCALE = 1.1f;
	constexpr float UNSELECT_COLOR = 0.6f;

	/** 「もどる」を選んでいないときに重ねる暗い画像の濃さ */
	constexpr float BACK_DARK_ALPHA = 0.35f;

}

bool Option::Start()
{
	/** 背景 */
	m_background.Init(BACKGROUND_PATH, BACKGROUND_W, BACKGROUND_H);
	m_background.SetPosition(Vector3::Zero);
	m_background.Update();

	/** 音量の棒を、BGMとSEの2行ぶん作る。緑と黒は同じ位置に重ねて置く */
	for (int row = 0; row < en_VolumeNum; row++)
	{
		for (int i = 0; i < BAR_NUM; i++)
		{
			const Vector3 pos(BAR_START_X + BAR_SPACING * i, VOLUME_ROW_Y[row], 0.0f);

			m_barGreen[row][i].Init(BAR_PATH, BAR_W, BAR_H);
			m_barGreen[row][i].SetPosition(pos);
			m_barGreen[row][i].Update();

			m_barBlack[row][i].Init(BLACK_BAR_PATH, BAR_W, BAR_H);
			m_barBlack[row][i].SetPosition(pos);
			m_barBlack[row][i].Update();
		}
	}

	/** 項目名の画像(横幅は文字数に合わせた仮の値) */
	m_BGMFontRender.Init(BGM_PATH, FONT_W * 6.0f, FONT_H * 1.2f);
	m_BGMFontRender.SetPosition(Vector3(LABEL_X, ROW_BGM_Y, 0.0f));
	m_BGMFontRender.Update();

	m_SEFontRender.Init(SE_PATH, FONT_W * 6.0f, FONT_H * 1.2f);
	m_SEFontRender.SetPosition(Vector3(LABEL_X, ROW_SE_Y, 0.0f));
	m_SEFontRender.Update();

	m_vibrationRender.Init(VIBRATION_PATH, FONT_W * 10.0f, FONT_H);
	m_vibrationRender.SetPosition(Vector3(LABEL_X + 30.0f, ROW_VIBRATION_Y, 0.0f));
	m_vibrationRender.Update();

	/** バイブレーションのスイッチ画像。ONとOFFは同じ位置に置く */
	const Vector3 switchPos(SWITCH_X, ROW_VIBRATION_Y, 0.0f);

	const Vector3 comeBackPos(COMEBACK_X, ROW_COMEBACK_Y, 0.0f);

	m_switchON.Init(ONSWITCH_PATH, SWITCH_W, SWITCH_H);
	m_switchON.SetPosition(switchPos);
	m_switchON.Update();

	m_switchOFF.Init(OFFSWITCH_PATH, SWITCH_W, SWITCH_H);
	m_switchOFF.SetPosition(switchPos);
	m_switchOFF.Update();

	m_ComeBackRender.Init(COMEBACK_PATH, COMEBACK_W, COMEBACK_H);
	m_ComeBackRender.SetPosition(comeBackPos);
	m_ComeBackRender.Update();

	m_ComeBackBlackRender.Init(COMEBACKBLACK_PATH, COMEBACK_W, COMEBACK_H);
	m_ComeBackBlackRender.SetPosition(comeBackPos);
	m_ComeBackBlackRender.Update();

	return true;
}

void Option::Update()
{
	/** 選択の見た目は毎フレーム更新する */
	UpdateSelectView();

	/** 開いた瞬間のAは無視して連続して押してしまわないようにする */
	if (m_firstFrame)
	{
		m_firstFrame = false;
		return;
	}

	/** 閉じる合図を出した後は、入力を受け付けない */
	if (m_closed) return;

	MoveSelect();
	ChangeValue();

	/** Aで決定。「もどる」のときだけ閉じる合図を出す */
	if (g_pad[0]->IsTrigger(enButtonA) && m_select == en_ItemBack)
	{
		SoundManager::GetInstance()->PlaySE(SEType::en_DecideSE);
		m_closed = true;
	}
}

void Option::MoveSelect()
{
	bool up = g_pad[0]->IsTrigger(enButtonUp);
	bool down = g_pad[0]->IsTrigger(enButtonDown);

	/** スティック上下。倒したままでも1回だけ反応させる */
	const float stickY = g_pad[0]->GetLStickYF();
	if (fabsf(stickY) < STICK_TILT)
	{
		m_stickReleasedY = true;
	}
	else if (m_stickReleasedY)
	{
		if (stickY > 0.0f) up = true;
		else               down = true;
		m_stickReleasedY = false;
	}

	/** 上下の端でループする */
	const int old = m_select;
	if (up)   m_select = (m_select + en_ItemNum - 1) % en_ItemNum;
	if (down) m_select = (m_select + 1) % en_ItemNum;

	/** 選択が変わったときだけSEを鳴らす */
	if (m_select != old)
	{
		SoundManager::GetInstance()->PlaySE(SEType::en_SelectSE);
	}
}

void Option::ChangeValue()
{
	/** 「もどる」には左右+の操作はない */
	if (m_select == en_ItemBack) return;

	int dir = 0;
	if (g_pad[0]->IsTrigger(enButtonLeft))  dir = -1;
	if (g_pad[0]->IsTrigger(enButtonRight)) dir = +1;

	/** スティック左右 */
	const float stickX = g_pad[0]->GetLStickXF();
	if (fabsf(stickX) < STICK_TILT)
	{
		m_stickReleasedX = true;
	}
	else if (m_stickReleasedX)
	{
		dir = (stickX > 0.0f) ? +1 : -1;
		m_stickReleasedX = false;
	}

	if (dir == 0) return;

	if (m_select == en_ItemVibration)
	{
		ToggleVibration();
	}
	else
	{
		/** en_ItemBGM と en_ItemSE は VolumeType と同じ番号 */
		AddVolume(m_select, dir);
	}
}

void Option::AddVolume(int volumeType, int dir)
{
	SetteiManager* setting = SetteiManager::GetInstance();

	if (volumeType == en_BGM)
	{
		const int old = setting->GetBGMLevel();
		setting->SetBGMLevel(old + dir);

		/** 値が変わったときだけ、再生中のBGMに反映する */
		if (setting->GetBGMLevel() != old)
		{
			SoundManager::GetInstance()->UpdateBGMVolume();
		}
	}
	else
	{
		const int old = setting->GetSELevel();
		setting->SetSELevel(old + dir);

		/** 値が変わったときだけ、新しい音量で確認用のSEを鳴らす */
		if (setting->GetSELevel() != old)
		{
			SoundManager::GetInstance()->PlaySE(SEType::en_SelectSE);
		}
	}
}

void Option::ToggleVibration()
{
	SetteiManager* setting = SetteiManager::GetInstance();
	setting->SetVibration(!setting->IsVibration());

	/** ONにした瞬間だけ、確認用に軽く振動させる */
	if (setting->IsVibration())
	{
		Vibrate();
	}
	SoundManager::GetInstance()->PlaySE(SEType::en_SelectSE);
}

void Option::Vibrate()
{
	/** ONにした確認用の振動。軽く、短く */
	SetteiManager::GetInstance()->Vibrate(0.3f, 1.0f);
}

void Option::UpdateSelectView()
{
	/** 項目名の画像を、並びの順(en_ItemBGM, en_ItemSE, en_ItemVibration)で扱う */
	SpriteRender* labels[3] = { &m_BGMFontRender, &m_SEFontRender, &m_vibrationRender };

	for (int i = 0; i < 3; i++)
	{
		const bool sel = (m_select == i);
		const float s = sel ? SELECT_SCALE : 1.0f;
		const float c = sel ? 1.0f : UNSELECT_COLOR;

		labels[i]->SetScale(Vector3(s, s, 1.0f));
		labels[i]->SetMulColor(Vector4(c, c, c, 1.0f));
		labels[i]->Update();
	}

	/** バイブレーションのスイッチも、選択中は大きく明るくする */
	{
		const bool sel = (m_select == en_ItemVibration);
		const float s = sel ? SELECT_SCALE : 1.0f;
		const float c = sel ? 1.0f : UNSELECT_COLOR;

		m_switchON.SetScale(Vector3(s, s, 1.0f));
		m_switchON.SetMulColor(Vector4(c, c, c, 1.0f));
		m_switchON.Update();
		m_switchOFF.SetScale(Vector3(s, s, 1.0f));
		m_switchOFF.SetMulColor(Vector4(c, c, c, 1.0f));
		m_switchOFF.Update();
	}

	/** 「もどる」: 選択中は大きく、選んでいないときは暗い画像を重ねる */
	{
		const bool sel = (m_select == en_ItemBack);
		const float s = sel ? SELECT_SCALE : 1.0f;

		m_ComeBackRender.SetScale(Vector3(s, s, 1.0f));
		m_ComeBackRender.Update();

		m_ComeBackBlackRender.SetScale(Vector3(s, s, 1.0f));
		m_ComeBackBlackRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, BACK_DARK_ALPHA));
		m_ComeBackBlackRender.Update();
	}
}

void Option::Render(RenderContext& rc)
{
	m_background.Draw(rc);

	SetteiManager* setting = SetteiManager::GetInstance();

	/** 今の設定値(点灯させる本数) */
	const int levels[en_VolumeNum] =
	{
		setting->GetBGMLevel(),
		setting->GetSELevel()
	};

	for (int row = 0; row < en_VolumeNum; row++)
	{
		for (int i = 0; i < BAR_NUM; i++)
		{
			/** 左から数えて設定値の本数までは緑、残りは黒を描く */
			if (i < levels[row])
			{
				m_barGreen[row][i].Draw(rc);
			}
			else
			{
				m_barBlack[row][i].Draw(rc);
			}
		}
	}

	m_BGMFontRender.Draw(rc);
	m_SEFontRender.Draw(rc);
	m_vibrationRender.Draw(rc);
	m_ComeBackRender.Draw(rc);

	/** バイブレーションの設定に合わせて、ONかOFFのどちらか一方を描く */
	if (setting->IsVibration())
	{
		m_switchON.Draw(rc);
	}
	else
	{
		m_switchOFF.Draw(rc);
	}
}