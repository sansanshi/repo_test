#pragma once
#include<list>
#include<memory>
#include"Effect.h"
class Camera;
class EffectFactory
{
private:
	Camera& _cameraRef;
	std::list<std::shared_ptr<Effect>> _effects;

	int _hitEffectHandle;
	
public:
	EffectFactory(Camera&);
	~EffectFactory();

	std::list<std::shared_ptr<Effect>> GetEffects(){ return _effects; }
	void Create(Vector2);
	void Update();
	void Draw();
};

