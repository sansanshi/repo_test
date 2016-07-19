#pragma once
#include "Scene.h"
class TitleScene :
	public Scene
{
	enum State{
		state_none,
		state_start,
		state_ready,
	};
private:
	void (TitleScene::*_pFunc)();
public:
	TitleScene();
	~TitleScene();
};

