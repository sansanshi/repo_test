#include "GameMain.h"
#include <DxLib.h>
#include<math.h>

#include "PlayingScene.h"
#include"TitleScene.h"

GameMain::GameMain()
{
	DxLib::ChangeWindowMode(true);
	DxLib::SetGraphMode(640, 480, 32);
	if (DxLib_Init() == -1){
		return;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	_scene = nullptr;

	ChangeScene(new TitleScene());
}


GameMain::~GameMain()
{
}

void GameMain::Terminate()
{
	DxLib_End();
}

void GameMain::GameLoop()
{

	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();
		_scene->Update();
		ScreenFlip();
	}
}

void GameMain::ChangeScene(Scene* scene)
{
	delete _scene;
	_scene = scene;
}