#include "PlayingScene.h"
#include<DxLib.h>
#include"CollisionDetector.h"

//�Â����^�C�g�����
//���񂾂�ŏ�����n�܂�

PlayingScene::PlayingScene() : _player(_camera), _camera(_player), _enemyFac(_player, _camera), _blockFac(_player)
{
	_groundZero = 360.0f;
	_stageGrHandle = LoadGraph("img/stage.png");

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

	
	for (auto& enemy : _enemyFac.GetEnemies())//�G�l�~�[�ƃv���C���[�̂����蔻��
	{
		if (!enemy->IsCollidable() || !_player.IsAvailable()) continue;
		
		CollisionDetector::HitCheck(&_player, enemy);//����
	}

	//Collider col = _player.GetAttackCol();
	//Collider playerCol = _player.GetCollider();
	//GameObject& obj = _grabman;
	//Collider enCol = obj.GetCollider();
	//if (CollisionDetector::IsHit(_player.GetCollider(), _grabman.GetCollider()))
	//{
	//	_player.OnCollided(&(_grabman.GetCollider()));
	//}
	//if (CollisionDetector::IsHit(_player.GetAttackCol(), obj.GetCollider()))//��ŏ���
	//{
	//	obj.OnCollided(&(_player.GetAttackCol()));//����@&���ĎQ�Ɓi�A�h���X�j�ɂ��ēn��
	//	//obj.OnCollided(_player.GetCollider());
	//}

	//Enemy& enemy = _grabman;
	//if (CollisionDetector::IsHit(_player.GetCollider(), obj.GetCollider()))
	//{
	//	if(enemy.IsCollidable())_player.OnCollided(&enemy);
	//}

	_enemyFac.Draw();
	_player.Draw();
}

