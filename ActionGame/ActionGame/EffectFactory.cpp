#include "EffectFactory.h"
#include<Dxlib.h>
#include"Camera.h"
EffectFactory::EffectFactory(Camera& camera) :_cameraRef(camera)
{
	_hitEffectHandle = LoadGraph("img/hiteffect.png");
}


EffectFactory::~EffectFactory()
{
}


void
EffectFactory::Create(Vector2 pos)
{
	std::shared_ptr<Effect> ret;
	ret.reset(new Effect(pos, _hitEffectHandle, _cameraRef));
	_effects.push_back(ret);
}

void
EffectFactory::Update()
{
	for (auto& effect : _effects)
	{
		effect->Update();
	}


	_effects.remove_if(
		[](std::shared_ptr<Effect>& e){//ƒ‰ƒ€ƒ_Ž®‚ªŽg‚í‚ê‚Ä‚¢‚é
		return !e->IsAvailable();
	});

}
void
EffectFactory::Draw()
{
	for (auto& effect : _effects)
	{
		effect->Draw();
	}
}