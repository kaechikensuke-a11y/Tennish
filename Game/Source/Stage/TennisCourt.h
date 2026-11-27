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
	/** テニスのネット */
	ModelRender m_TennisNet;
	/** 審判台 */
	ModelRender m_UmpireChair;
};

