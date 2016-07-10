#include "PlayingScene.h"
#include<DxLib.h>
#include"CollisionDetector.h"

//�Â����^�C�g�����
//���񂾂�ŏ�����n�܂�

PlayingScene::PlayingScene() : _player(_camera,_stage), _camera(_player), _enemyFac(_player, _camera), _blockFac(_player), _stage(_camera)
{
	_groundZero = 360.0f;
	_stageGrHandle = LoadGraph("img/stage.png");
	Rect _hpBarRect = Rect(Vector2(50, 400), 400, 80);//HP�o�[�\���p�̋�`�@�`��ɂ����Left()�Ƃ��g��

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
		if (!enemy->GetCollider().IsCollidable() || !_player.IsAvailable()) continue;
		
		CollisionDetector::HitCheck(&_player, enemy);
	}

	if (CollisionDetector::IsHit(_player.GetCollider(), _stage.GetCollider()))//�v���C���[�̉����߂�
	{
		Vector2 vec=CollisionDetector::RejectVec(&_player.GetCollider(), &_stage.GetCollider());
		_player.Reject(vec);
	}

	for (auto& enemy : _enemyFac.GetEnemies())
	{
		if (!enemy->GetCollider().IsCollidable()) continue;//�����蔻��false�������画�肹������
		if (CollisionDetector::IsHit(enemy->GetCollider(), _stage.GetCollider()))//�G�l�~�[�̉����߂�
		{
			Vector2 vec = CollisionDetector::RejectVec(&enemy->GetCollider(), &_stage.GetCollider());
			enemy->Reject(vec);
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
	_stage.Draw();

	DrawExtendGraph(_chargeMeter.pos.x + _chargeMeter.barOffset.x, _chargeMeter.pos.y + _chargeMeter.barOffset.y,
		_chargeMeter.pos.x + _chargeMeter.barOffset.x + 256 * _chargeCnt / _chargeMax, _chargeMeter.pos.y + _chargeMeter.barOffset.y + 10, _chargeMeter.barHandle, false);//�o�[�\�� �����̂�2�s

}

