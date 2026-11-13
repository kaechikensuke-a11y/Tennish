#pragma once
class ResultMenu : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	bool IsClosed() const { return m_closed; }
	int GetDecided() const { return m_decided; }
private:
	static constexpr int MENU_NUM = 2;
	SpriteRender m_blackOut;
	SpriteRender m_button[MENU_NUM];

	float m_timer = 0.0f;
	int m_index = 0;
	int m_decided = -1;
	bool m_closed = false;
	/** 開いた瞬間の入力は無視させる */
	bool m_firstFrame = true;

};

