#include "PlayingScene.h"
#include<DxLib.h>
#include"CollisionDetector.h"
#include<assert.h>
#include"GameMain.h"
#include"Block.h"

//�Â����^�C�g�����
//���񂾂�ŏ�����n�܂�

PlayingScene::PlayingScene() 
: _player(_camera, _stage), _camera(_player), _enemyFac(_player, _camera,_ebulletFac), _blockFac(_player,_camera), _stage(_camera),_ebulletFac(_camera),_effectFac(_camera)/*, _fragDrawer()*/
{
	_groundZero = 360.0f;
	_stageGrHandle = LoadGraph("img/stage.png");
	_hpBarRect = Rect(Vector2(50, 20), 400, 20);//HP�o�[�\���p�̋�`�@�`��ɂ����Left()�Ƃ��g��
	_hpBarHandle = LoadGraph("img/meter_bar.png");

	_camera.SetUp();

	rect = Rect(Vector2(960, 380), 500, 100);


	testHandle = LoadGraph("img/stage.png");
	shaderHandle = LoadPixelShader("model/sakuya.pso");
	thirdscreen = MakeScreen(640, 480);

	//���_�̐ݒ�
	for (int i = 0; i < 4; i++)
	{
		vertex[i].pos = VGet((i % 2)*640.0f, (i / 2)*480.0f, 0);
		vertex[i].rhw = 1.0f;
		vertex[i].dif = GetColorU8(255, 255, 255, 255);
		vertex[i].spc = GetColorU8(0, 0, 0, 0);
		vertex[i].u = vertex[i].su = (float)(i % 2);
		vertex[i].v = vertex[i].sv = (float)(i / 2);
	}

	_timer = 0;
}


PlayingScene::~PlayingScene()
{
}

void
PlayingScene::Update()
{
	++_timer;
	_camera.Update();
	_player.Update();
	/*if (_player.GetRect().Bottom() >= _groundZero)
	{
		Vector2 temp=_player.GetPos();
		_player.SetPos(Vector2(temp.x, _groundZero));

		
	}*/

	_stage.Update();
	
	_enemyFac.Update();
	_blockFac.Update();
	_ebulletFac.Update();
	_effectFac.Update();

	
	//DrawExtendGraph()

	
	for (auto& enemy : _enemyFac.GetEnemies())//�G�l�~�[�ƃv���C���[�̂����蔻��
	{
		if (!enemy->GetCollider().IsCollidable() || !_player.IsAvailable()) continue;
		
		CollisionDetector::HitCheck(&_player, enemy);
	}

	for (auto& ebullet : _ebulletFac.GetEnemyBullets())//�G�l�~�[�̒e�ƃv���C���[�̂����蔻��
	{
		if (!ebullet->GetCollider().IsCollidable() || !_player.IsAvailable()) continue;

		if(CollisionDetector::IsHit(_player.GetCollider(),ebullet->GetCollider())) ebullet->OnCollided(&(_player.GetCollider()));
	}


	if (CollisionDetector::IsHit(_player.GetCollider(), _stage.GetCollider()))//�v���C���[�̉����߂�
	{
		Vector2 vec=CollisionDetector::RejectVec(&_player.GetCollider(), &_stage.GetCollider());
		_player.Reject(vec);//������Reject�̒��ŏ�ɉ����߂����ꍇstate��walk�ɂ�����Ă������������Ă邩��Grabbed����ς���Ă�
	}

	for (auto& enemy : _enemyFac.GetEnemies())//�G�l�~�[���m�̂����蔻��
	{
		if (!enemy->GetCollider().IsCollidable() ) continue;//�G�l�~�[�������̓v���C���[�̍U����Collider�������������画��͂��Ȃ�
		for (auto& enemy2 : _enemyFac.GetEnemies()){
			if (enemy2 == enemy) continue;//�������g�Ƃ̂����蔻��͔�΂�
			//if (!enemy2->GetCollider().IsCollidable()) continue;
			if (CollisionDetector::IsHit(enemy->GetCollider(), enemy2->GetCollider()))
			{
				enemy->OnCollided(&(enemy2->GetCollider()));
			}
		}
	}

	for (auto& enemy : _enemyFac.GetEnemies())//�G�l�~�[�ƃX�e�[�W�̓����蔻��i�����߂��j
	{
		if (!enemy->GetCollider().IsCollidable()) continue;//�����蔻��false�������画�肹������
		if (CollisionDetector::IsHit(enemy->GetCollider(), _stage.GetCollider()))//�G�l�~�[�̉����߂�
		{
			Vector2 vec = CollisionDetector::RejectVec(&enemy->GetCollider(), &_stage.GetCollider());
			enemy->Reject(vec);
		}
	}
	for (auto& enemy : _enemyFac.GetEnemies())//�G�l�~�[�ƃv���C���[�̍U���̓����蔻��
	{
		if (!enemy->GetCollider().IsCollidable()||!_player.GetAttackCol().IsCollidable()) continue;//�G�l�~�[�������̓v���C���[�̍U����Collider�������������画��͂��Ȃ�
		if (CollisionDetector::IsHit(enemy->GetCollider(), _player.GetAttackCol()))
		{
			enemy->OnCollided(&(_player.GetAttackCol()));
			_camera.InvokeQuake(5.0f);
			if(enemy->CharaType()==ct_knifeMan)_blockFac.CreateBlock(bt_vmovable, enemy->GetPos()+Vector2(0,30));//
			if (enemy->CharaType() == ct_grabMan)_blockFac.CreateBlock(bt_movable, enemy->GetPos() + Vector2(0, 30));

			Vector2 pos = CollisionDetector::CenterOfHit(enemy->GetCollider(), _player.GetAttackCol());
			int x = _camera.OffsetX();
			int a = 0;
			
			_effectFac.Create(CollisionDetector::CenterOfHit(enemy->GetCollider(), _player.GetAttackCol()));
		}
	}
	
	for (auto& enemy : _enemyFac.GetEnemies())
	{
		if (!enemy->CharaType() == ct_batman) continue;
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
				
				if ((block->GetBlockType()==bt_movable||block->GetBlockType()==bt_vmovable)&&vec.y < 0.0f) block->OnCollided(_player);//������ɉ����߂���Ă����灩���܂������ĂȂ�
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

	SetDrawScreen(thirdscreen);
	ClearDrawScreen();
	DrawExtendGraph(0 + _camera.OffsetX() - 640, 0, 0 + _camera.OffsetX(), 480, _stageGrHandle, false);
	DrawExtendGraph(0 + _camera.OffsetX(), 0, 640 + _camera.OffsetX(), 480, _stageGrHandle, false);
	DrawExtendGraph(0 + _camera.OffsetX() + 640, 0, 640 + _camera.OffsetX() + 640, 480, _stageGrHandle, false);

	SetDrawScreen(DX_SCREEN_BACK);
	SetPSConstSF(0, _timer);
	SetUseTextureToShader(0, thirdscreen);
	//�s�N�Z���V�F�[�_�̃Z�b�g
	SetUsePixelShader(shaderHandle);
	DrawPrimitive2DToShader(vertex, 4, DX_PRIMTYPE_TRIANGLESTRIP);

	//ScreenFlip();
	//ClearDrawScreen();

	DrawBox(rect.Left() + _camera.OffsetX(), rect.Top(), rect.Right() + _camera.OffsetX(), rect.Bottom(), 0x000000, true);

	_enemyFac.Draw();
	_player.Draw();
	_stage.Draw();
	_blockFac.Draw();
	_ebulletFac.Draw();
	_effectFac.Draw();

	//_fragDrawer.Draw();

	DrawExtendGraph(_hpBarRect.Left(),_hpBarRect.Top() ,
		_hpBarRect.Left() + _hpBarRect.width * _player.GetPercentageHp(), _hpBarRect.Bottom(), _hpBarHandle, false);//�o�[�\�� �����̂�2�s


	

	_player.IsDead() ? GameMain::Instance().ChangeScene(new PlayingScene()):0;//�v���C���[������ł���V�[�������[�h�iscene��delete���Ă�̂ŏ����̈�ԍŌ�ɒu���j

}


