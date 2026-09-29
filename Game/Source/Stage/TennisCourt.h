#pragma once
class TennisCourt : public IGameObject
{
public:

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/** 通常のテニスコートのステージ */
	ModelRender m_normalTennisCourt;
};

