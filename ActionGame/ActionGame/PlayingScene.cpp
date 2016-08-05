#include "PlayingScene.h"
#include<DxLib.h>
#include"CollisionDetector.h"
#include<assert.h>
#include"GameMain.h"
#include"Block.h"
#include<string>

//凝ったタイトル作る
//死んだら最初から始まる

PlayingScene::PlayingScene() 
: _player(_camera, _stage,*this), _camera(_player), _enemyFac(_player, _camera,_ebulletFac), _blockFac(_player,_camera), _stage(_camera),_ebulletFac(_camera),_effectFac(_camera), _fragDrawer()
{
	_groundZero = 360.0f;
	_stageGrHandle = LoadGraph("img/stage.png");
	_hpBarRect = Rect(Vector2(50, 20), 400, 20);//HPバー表示用の矩形　描画にこれのLeft()とか使う
	_hpBarHandle = LoadGraph("img/meter_bar.png");

	_camera.SetUp();

	rect = Rect(Vector2(960, 380), 500, 100);


	testHandle = LoadGraph("img/stage.png");
	shaderHandle = LoadPixelShader("shader/sakuya.pso");
	thirdscreen = MakeScreen(640, 480);

	//頂点の設定
	for (int i = 0; i < 4; i++)
	{
		vertex[i].pos = VGet((i % 2)*640.0f, (i / 2)*480.0f, 0);
		vertex[i].rhw = 1.0f;
		vertex[i].dif = GetColorU8(255, 255, 255, 255);
		vertex[i].spc = GetColorU8(0, 0, 0, 0);
		vertex[i].u = vertex[i].su = (float)(i % 2);
		vertex[i].v = vertex[i].sv = (float)(i / 2);
	}

	_timer = 0.f;
	_timerAccel = 0.f;
	_fragDrawer.InitVertices();//コンストラクタで走るCreateVerticesだと頂点カラー弄ってるのでそれ直す
	_isTimeBreaking = false;
	_isTimeStopping = false;
	_isTimeStop = false;
	_isTimeMove = true;

	_stoppingTimer = 0;
	_stoppingLimit = 120;
}


PlayingScene::~PlayingScene()
{
}

void
PlayingScene::Update()
{
	if (_isTimeStopping){
		_timer = min(_timer + _timerAccel, 100.0f);
		_timerAccel += 0.15f;
		if (_timer == 100.0f) {
			_isTimeMove = false;
			_isTimeStopping = false;
			_isTimeStop = true;
		}
	}
	if (_isTimeBreaking)
	{
		_isTimeStop = false;
		_timer = max(_timer - _timerAccel, 0.0f);
		_timerAccel -= 0.15f;
		if (_timer == 0.0f) {
			_isTimeBreaking = false;
			_isTimeMove = true;
		}
	}

	if (_isTimeStop)
	{
		++_stoppingTimer;
		if (_stoppingTimer > _stoppingLimit)
		{
			_stoppingTimer = 0;
			_isTimeBreaking = true;
		}
	}

	_camera.Update();
	_player.Update();
	/*if (_player.GetRect().Bottom() >= _groundZero)
	{
		Vector2 temp=_player.GetPos();
		_player.SetPos(Vector2(temp.x, _groundZero));

		
	}*/

	_stage.Update();
	
	if(_timer<100.0f)_enemyFac.Update();
	if(_timer<100.0f)_blockFac.Update();
	if (_timer<100.0f)_ebulletFac.Update();
	_effectFac.Update();

	
	//DrawExtendGraph()

	
	for (auto& enemy : _enemyFac.GetEnemies())//エネミーとプレイヤーのあたり判定
	{
		if (!enemy->GetCollider().IsCollidable() || !_player.IsAvailable()) continue;
		
		CollisionDetector::HitCheck(&_player, enemy);
	}

	for (auto& ebullet : _ebulletFac.GetEnemyBullets())//エネミーの弾とプレイヤーのあたり判定
	{
		if (!ebullet->GetCollider().IsCollidable() || !_player.IsAvailable()) continue;

		if(CollisionDetector::IsHit(_player.GetCollider(),ebullet->GetCollider())) ebullet->OnCollided(&(_player.GetCollider()));
	}


	if (CollisionDetector::IsHit(_player.GetCollider(), _stage.GetCollider()))//プレイヤーの押し戻し
	{
		Vector2 vec=CollisionDetector::RejectVec(&_player.GetCollider(), &_stage.GetCollider());
		_player.Reject(vec);//ここのRejectの中で上に押し戻した場合stateをwalkにするっていう処理をしてるからGrabbedから変わってる
	}

	for (auto& enemy : _enemyFac.GetEnemies())//エネミー同士のあたり判定
	{
		if (!enemy->GetCollider().IsCollidable() ) continue;//エネミーもしくはプレイヤーの攻撃のColliderが無効だったら判定はしない
		for (auto& enemy2 : _enemyFac.GetEnemies()){
			if (enemy2 == enemy) continue;//自分自身とのあたり判定は飛ばす
			//if (!enemy2->GetCollider().IsCollidable()) continue;
			if (CollisionDetector::IsHit(enemy->GetCollider(), enemy2->GetCollider()))
			{
				enemy->OnCollided(&(enemy2->GetCollider()));
			}
		}
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
			if(enemy->CharaType()==ct_knifeMan)_blockFac.CreateBlock(bt_vmovable, enemy->GetPos()+Vector2(0,30));//
			if (enemy->CharaType() == ct_grabMan)_blockFac.CreateBlock(bt_movable, enemy->GetPos() + Vector2(0, 30));

			//TimeBreak();//ここにくるまでに何も描画してないのでGetDrawScreenしても真っ黒な画面しか撮れない
			//if(_isTimeStopping)_timeBreaking = true;

			Vector2 pos = CollisionDetector::CenterOfHit(enemy->GetCollider(), _player.GetAttackCol());
			int x = _camera.OffsetX();
			int a = 0;
			
			_effectFac.Create(CollisionDetector::CenterOfHit(enemy->GetCollider(), _player.GetAttackCol()));
		}
	}
	
	for (auto& enemy : _enemyFac.GetEnemies())
	{
		if (!(enemy->CharaType() == ct_batman)) continue;
		if (_player.IsAvailable() && enemy->GetAttackCol().IsCollidable())
		{
			if (CollisionDetector::IsHit(_player.GetCollider(), enemy->GetAttackCol()))
			{
				_player.OnCollided(&(enemy->GetAttackCol()));
			}
		}
		
	}

	for (auto& block : _blockFac.GetBlocks())
	{
		if (_player.IsAvailable()&&_player.GetRejectCnt()<2)
		{
			if (CollisionDetector::IsHit(_player.GetCollider(), block->GetCollider()))
			{
				Vector2 vec = CollisionDetector::RejectVec(&_player.GetCollider(), &(block->GetCollider()));
				_player.Reject(vec);
				
				if ((block->GetBlockType()==bt_movable||block->GetBlockType()==bt_vmovable)&&vec.y < 0.0f) block->OnCollided(_player);//上方向に押し戻されていたら←うまくいってない
			}
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

	SetDrawScreen(thirdscreen);
	ClearDrawScreen();
	DrawExtendGraph(0 + _camera.OffsetX() - 640, 0, 0 + _camera.OffsetX(), 480, _stageGrHandle, false);
	DrawExtendGraph(0 + _camera.OffsetX(), 0, 640 + _camera.OffsetX(), 480, _stageGrHandle, false);
	DrawExtendGraph(0 + _camera.OffsetX() + 640, 0, 640 + _camera.OffsetX() + 640, 480, _stageGrHandle, false);

	_enemyFac.Draw();
	//_player.Draw();
	_stage.Draw();
	_blockFac.Draw();
	_ebulletFac.Draw();
	_effectFac.Draw();

	SetDrawScreen(DX_SCREEN_BACK);
	SetPSConstSF(0, _timer);
	SetUseTextureToShader(0, thirdscreen);
	//ピクセルシェーダのセット
	SetUsePixelShader(shaderHandle);
	DrawPrimitive2DToShader(vertex, 4, DX_PRIMTYPE_TRIANGLESTRIP);

	MV1SetUseOrigShader(false);

	DrawBox(rect.Left() + _camera.OffsetX(), rect.Top(), rect.Right() + _camera.OffsetX(), rect.Bottom(), 0x000000, true);

	_player.Draw();//プレイヤーは白黒にしたくないので2回描画してみる
	
	//_fragDrawer.Capture();
	if (_isTimeBreaking)
	{
		//TimeBreak();
		//_timeBreaking = false;
	}
	_fragDrawer.Draw();
	

	DrawExtendGraph(_hpBarRect.Left(),_hpBarRect.Top() ,
		_hpBarRect.Left() + _hpBarRect.width * _player.GetPercentageHp(), _hpBarRect.Bottom(), _hpBarHandle, false);//バー表示 長いので2行

	DrawString(30, 200, "移動：十字キー\nパンチ：Z　キック：X\n時間停止、解除：A\nジャンプ：スペース",0xff0000);
	float f = (float)(_stoppingLimit - _stoppingTimer) / 60.f;
	std::string s = std::to_string(f);
	if(_isTimeStop)DrawString(560, 200, s.c_str(), 0x0000ff);
	

	_player.IsDead() ? GameMain::Instance().ChangeScene(new PlayingScene()):0;//プレイヤーが死んでたらシーンをロード（sceneをdeleteしてるので処理の一番最後に置く）

}

void
PlayingScene::TimeBreak()
{
	_isTimeStopping = false;
	_fragDrawer.InitVertices();
	_fragDrawer.Capture();
	_fragDrawer.Break();
	_timer = 0;
	_timerAccel = 0;
}
void
PlayingScene::TimeStop()
{
	_isTimeStopping = true;
}
void
PlayingScene::TimeMove()
{
	_isTimeBreaking = true;
}


