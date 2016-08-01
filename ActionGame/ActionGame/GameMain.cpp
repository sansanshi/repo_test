#include "GameMain.h"
#include <DxLib.h>
#include<math.h>

#include "PlayingScene.h"
#include"TitleScene.h"

GameMain::GameMain()
{
	SetUseDirect3DVersion(DX_DIRECT3D_9EX);//�f�t�H����directX11���g���Ă���@11���ƃV�F�[�_�̓ǂݍ��݂����܂������Ȃ����ۂ�
	DxLib::ChangeWindowMode(true);
	DxLib::SetGraphMode(640, 480, 32);
	if (DxLib_Init() == -1){
		return;
	}
	int i=GetUseDirect3DVersion();

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