#pragma once
class Option : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/** 「もどる」が決定されたら */
	bool IsClosed() const { return m_closed; }

private:
	/** 音量項目の種類(棒の行の番号としても使う) */
	enum VolumeType
	{
		en_BGM,
		en_SE,
		en_VolumeNum
	};

	/** 設定項目の種類 */
	enum Item
	{
		en_ItemBGM,
		en_ItemSE,
		en_ItemVibration,
		en_ItemBack,
		en_ItemNum
	};

	/** 棒の本数 */
	static constexpr int BAR_NUM = 10;

	/** 上下の入力で、選んでいる項目を動かす */
	void MoveSelect();

	/** 左右の入力で、選んでいる項目の値を変える */
	void ChangeValue();

	/** 選んでいる項目に合わせて、大きさと明るさを更新する */
	void UpdateSelectView();

	/** 値を1段階変える。dir は +1(右)か -1(左) */
	void AddVolume(int volumeType, int dir);
	void ToggleVibration();

	/** コントローラーを軽く振動させる */
	void Vibrate();

	/** 背景画像 */
	SpriteRender m_background;
	/** 音量の棒 */
	SpriteRender m_barGreen[en_VolumeNum][BAR_NUM];
	SpriteRender m_barBlack[en_VolumeNum][BAR_NUM];
	/** 文字の画像 */
	SpriteRender m_SEFontRender;
	SpriteRender m_BGMFontRender;
	SpriteRender m_vibrationRender;
	SpriteRender m_ComeBackRender;
	SpriteRender m_ComeBackBlackRender;
	/** バイブレーションのスイッチ画像 */
	SpriteRender m_switchON;
	SpriteRender m_switchOFF;

	/** 今選んでいる項目 */
	int  m_select = en_ItemBGM;
	/** 「もどる」が決定されたか */
	bool m_closed = false;
	/** 開いた瞬間の入力を無視するためのフラグ */
	bool m_firstFrame = true;
	/** スティックを戻したか(倒しっぱなしで連続入力にならないように) */
	bool m_stickReleasedY = true;
	bool m_stickReleasedX = true;
};

