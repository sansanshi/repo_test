#include "PlayingScene.h"
#include<DxLib.h>
#include"CollisionDetector.h"

//凝ったタイトル作る
//死んだら最初から始まる

PlayingScene::PlayingScene() : _player(_camera,_stage), _camera(_player), _enemyFac(_player, _camera), _blockFac(_player), _stage(_camera)
{
	_groundZero = 360.0f;
	_stageGrHandle = LoadGraph("img/stage.png");
	_hpBarRect = Rect(Vector2(50, 20), 400, 20);//HPバー表示用の矩形　描画にこれのLeft()とか使う
	_hpBarHandle = LoadGraph("img/meter_bar.png");

	_camera.SetUp();
}


PlayingScene::~PlayingScene()
{
}

void
PlayingScene::Update()
{
	_camera.Update();
	_player.Update();
	/*if (_player.GetRect().Bottom() >= _groundZero)
	{
		Vector2 temp=_player.GetPos();
		_player.SetPos(Vector2(temp.x, _groundZero));

		
	}*/

	
	_enemyFac.Update();


	DrawExtendGraph(0 + _camera.OffsetX()-640, 0, 640 + _camera.OffsetX(), 480, _stageGrHandle, false);
	DrawExtendGraph(0+_camera.OffsetX(), 0,640+_camera.OffsetX(),480, _stageGrHandle, false);

	
	for (auto& enemy : _enemyFac.GetEnemies())//エネミーとプレイヤーのあたり判定
	{
		if (!enemy->GetCollider().IsCollidable() || !_player.IsAvailable()) continue;
		
		CollisionDetector::HitCheck(&_player, enemy);
	}

	if (CollisionDetector::IsHit(_player.GetCollider(), _stage.GetCollider()))//プレイヤーの押し戻し
	{
		Vector2 vec=CollisionDetector::RejectVec(&_player.GetCollider(), &_stage.GetCollider());
		_player.Reject(vec);
	}

	for (auto& enemy : _enemyFac.GetEnemies())//エネミーとステージの当たり判定（押し戻し）
	{
		if (!enemy->GetCollider().IsCollidable()) continue;//当たり判定falseだったら判定せず次へ
		if (CollisionDetector::IsHit(enemy->GetCollider(), _stage.GetCollider()))//エネミーの押し戻し
		{
			Vector2 vec = CollisionDetector::RejectVec(&enemy->GetCollider(), &_stage.GetCollider());
			enemy->Reject(vec);
		}
	}
	for (auto& enemy : _enemyFac.GetEnemies())//エネミーとプレイヤーの攻撃の当たり判定
	{
		if (!enemy->GetCollider().IsCollidable()||!_player.GetAttackCol().IsCollidable()) continue;//エネミーもしくはプレイヤーの攻撃のColliderが無効だったら判定はしない
		if (CollisionDetector::IsHit(enemy->GetCollider(), _player.GetAttackCol()))
		{
			enemy->OnCollided(&(_player.GetAttackCol()));
			_camera.InvokeQuake(5.0f);
		}
	}

	//Collider col = _player.GetAttackCol();
	//Collider playerCol = _player.GetCollider();
	//GameObject& obj = _grabman;
	//Collider enCol = obj.GetCollider();
	//if (CollisionDetector::IsHit(_player.GetCollider(), _grabman.GetCollider()))
	//{
	//	_player.OnCollided(&(_grabman.GetCollider()));
	//}
	//if (CollisionDetector::IsHit(_player.GetAttackCol(), obj.GetCollider()))//後で消す
	//{
	//	obj.OnCollided(&(_player.GetAttackCol()));//これ　&つけて参照（アドレス）にして渡す
	//	//obj.OnCollided(_player.GetCollider());
	//}

	//Enemy& enemy = _grabman;
	//if (CollisionDetector::IsHit(_player.GetCollider(), obj.GetCollider()))
	//{
	//	if(enemy.IsCollidable())_player.OnCollided(&enemy);
	//}

	_enemyFac.Draw();
	_player.Draw();
	_stage.Draw();

	DrawExtendGraph(_hpBarRect.Left(),_hpBarRect.Top() ,
		_hpBarRect.Left() + _hpBarRect.width * _player.GetPercentageHp(), _hpBarRect.Bottom(), _hpBarHandle, false);//バー表示 長いので2行

}

