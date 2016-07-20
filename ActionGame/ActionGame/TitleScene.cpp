#include "TitleScene.h"
#include"Dxlib.h"
#include"PlayingScene.h"
#include"GameMain.h"
#include"FragmentDrawer.h"


TitleScene::TitleScene() 
{
	_handle = LoadGraph("img/title.png");
	_pFunc = &TitleScene::StartUpdate;
	_fragDrawer = new FragmentDrawer(_handle);

	_fragDrawer->FragmentScalling(0.8f);//Scallingの方を先にする
	_fragDrawer->FragmentTranslation(Vector2(-50, 0));

	_fragDrawer->TransparentFragment();
	_breakingTimer = 0;
	_breakFlag = false;
}


TitleScene::~TitleScene()
{
}
void
TitleScene::StartUpdate()
{
	++_startTimer;
	GetHitKeyStateAll(_key);
	if (_fragDrawer->GetAlpha()>=255)
	{
		_pFunc = &TitleScene::ReadyUpdate;
	}
	//DrawRectRotaGraph2(0, 0, 0, 0, 893, 237,0,0,0.8f,0.0f, _handle, true, false);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,254);
	_fragDrawer->IncrFragAlpha();
	_fragDrawer->Draw();
	//_fragDrawer->GetAlpha();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	if((_startTimer%20)/10==0)DrawString(290, 380, "Wait", 0xffffff);
}
void
TitleScene::ReadyUpdate()
{
	GetHitKeyStateAll(_key);
	if (_key[KEY_INPUT_RETURN])
	{
		_fragDrawer->Break();
		_breakFlag = true;
		//GameMain::Instance().ChangeScene(new PlayingScene());//これでシーンをデリートするのでこれ以降TitleSceneの処理は呼び出せなくなる
	}
	//DrawRectGraph(0, 0, 0, 0, 893, 237, _handle, true, false);
	_fragDrawer->Draw();
	DrawString(270, 380, "Press Enter", 0xffffff);
	_breakFlag ? ++_breakingTimer:0;
	if (_breakingTimer > 60)
	{
		GameMain::Instance().ChangeScene(new PlayingScene());//これでシーンをデリートするのでこれ以降TitleSceneの処理は呼び出せなくなる
	}
}

void
TitleScene::Update()
{
	SetDrawScreen(DX_SCREEN_BACK);
	(this->*_pFunc)();
}