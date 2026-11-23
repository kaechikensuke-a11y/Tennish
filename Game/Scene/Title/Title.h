#pragma once
class Option;
class Title :public IGameObject
{
public:
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/** タイトル画面 */
	enum class Menu
	{
		en_Start,
		en_Settei,
		en_Num
	};

	/** 上下の入力で選択を動かす */
	void MoveSelect();

	/** Bボタンで決定した時の処理 */
	void Decide();

	SpriteRender m_titleRender; /** タイトル画面 */
	SpriteRender m_startRender; /** 開始画面 */
	SpriteRender m_setteiRender;/** 設定画面 */

	/** 今選んでいる項目 */
	Menu m_select = Menu::en_Start;
	/** スティックを元に戻しているか */
	bool m_stickReleased = true;

	Option* m_option = nullptr;
	
};

