#pragma once
class PauseMenu : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/** 決定された項目の番号を返す。まだなら -1 */
	int GetDecided() const { return m_decided; }

	/** 項目の番号 */
	enum Item
	{
		en_Resume,
		en_Option,
		en_Title,
		en_ItemNum
	};

private:
	/** 暗幕 */
	SpriteRender m_blackOut;
	/** 項目の画像 */
	SpriteRender m_button[en_ItemNum];

	/** フェードインの経過時間 */
	float m_timer = 0.0f;
	/** 今選んでいる項目 */
	int m_index = en_Resume;
	/** 決定された項目(-1は未決定) */
	int m_decided = -1;
	/** 開いた瞬間の入力を無視するためのフラグ */
	bool m_firstFrame = true;
	/** スティックを戻したか */
	bool m_stickReleased = true;
};

