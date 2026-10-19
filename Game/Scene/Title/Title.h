#pragma once
class Title :public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	SpriteRender m_titleRender;

};

