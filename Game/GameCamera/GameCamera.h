#pragma once
class GameCamera : public IGameObject
{
public:
	GameCamera(){}
	~GameCamera(){}

public:
	bool Start();
	void Update();

private:
	Vector3 m_CameraPos;
	Vector3 m_CameraTarget;
};

