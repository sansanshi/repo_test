#pragma once
#include "Scene.h"
#include"FragmentDrawer.h"
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
	int _handle;
	char _key[256];
	FragmentDrawer *_fragDrawer;
	int _startTimer;
	int _breakingTimer;
	bool _breakFlag;
public:
	TitleScene();
	~TitleScene();
	void StartUpdate();
	void ReadyUpdate();
	void Update();
};

