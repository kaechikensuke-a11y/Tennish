#pragma once
class GameCamera : public IGameObject
{
public:
	GameCamera(){}
	~GameCamera(){}

public:
	bool Start();
	void Update();

	/** 演出用にカメラを切り替える */
	void SetCamera(const Vector3& pos, const Vector3& target)
	{
		m_CameraPos = pos;
		m_CameraTarget = target;
	}

private:
	Vector3 m_CameraPos;
	Vector3 m_CameraTarget;
};

