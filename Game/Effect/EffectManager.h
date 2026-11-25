#pragma once
class EffectEmitter;

/** エフェクトの種類 */
enum class EffectType
{
	en_ChargeFirst,
	en_ChargeSecond,
	en_ChargeThird,
	en_Num
};
class EffectManager : public IGameObject
{
public:
	static EffectManager* GetInstance() { return m_instance; }

	bool Start();

	/** エフェクトを出す */
	int Play(EffectType type, const Vector3& pos, float scale = 1.0f);

	/** 出しているエフェクトの位置と大きさを変える */
	void Move(int handle, const Vector3& pos, float scale = 1.0f);

	/** エフェクトを止める */
	void Stop(int handle);
private:
	static EffectManager* m_instance;
};

