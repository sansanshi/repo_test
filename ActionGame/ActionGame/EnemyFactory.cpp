#include "EnemyFactory.h"
#include<Dxlib.h>
#include"GrabMan.h"
#include"KnifeMan.h"
#include"Player.h"

EnemyFactory::EnemyFactory(Player& player,Camera& camera) :_playerRef(player), _cameraRef(camera)
{
	_enemyImgMap[et_grabman] = LoadGraph("img/grabman_.png");
	_enemyImgMap[et_knifeman] = LoadGraph("img/knifeman_.png");
	_enemyImgMap[et_batman] = LoadGraph("img/boss1_.png");
	
	_enemydeadImgMap[et_grabman] = LoadGraph("img/grabman_dead.png");
	_enemydeadImgMap[et_knifeman] = LoadGraph("img/knifeman_dead.png");
	_timer = 0;
}



EnemyFactory::~EnemyFactory()
{
	for (auto& h : _enemyImgMap){//�C���[�W�n���h���̍폜
		DeleteGraph(h.second);
	}

	//for (auto& enemy : _enemies)//�V�F�A�h�|�C���^�͒N���Q�Ƃ��Ȃ��Ȃ������_�ŏ���ɍ폜�����̂ł����͂���Ȃ�
	//{
	//	delete enemy;
	//}
}

std::shared_ptr<Enemy>
EnemyFactory::Create(EnemyType et, Vector2& pos)
{
	std::shared_ptr<Enemy> ret;
	switch (et)
	{
	case et_none:
		break;
	case et_grabman:
		ret.reset(new GrabMan(pos,_enemyImgMap[et], _enemydeadImgMap[et], _playerRef,_cameraRef));
		_enemies.push_back(ret);
		return _enemies.back();
		break;
	case et_knifeman:
		ret.reset(new KnifeMan(pos, _enemyImgMap[et], _enemydeadImgMap[et], _playerRef,_cameraRef));
		_enemies.push_back(ret);
		return _enemies.back();
		break;
	case et_batman:
		/*_enemies.push_back(new (pos, _enemyImgMap[et], _expHandle, _bgChips, _bgScroll, _eBulletFactory, _playerPos));
		return _enemies.back();*/
		break;
	default:
		return nullptr;
		break;
	}
}


void
EnemyFactory::Update()
{
	_timer++;
	if (_timer % 60 == 0){
		Create(et_grabman, _playerRef.GetPos() + Vector2(300, 0));
		Vector2 pos = _playerRef.GetPos() + Vector2(300, 0);
		int a = 0;
	}

	//�X�V���[�v
	for (auto e : _enemies)
	{
		e->Update();
	}
	
	//�s�v�Ȃ��̍폜���[�v
	_enemies.remove_if(
		[](std::shared_ptr<Enemy>& e){//�����_�����g���Ă���
		return !e->IsAvailable();
	});
	
}

std::list<std::shared_ptr<Enemy>>&
EnemyFactory::GetEnemies()
{
	return _enemies;
}

void
EnemyFactory::Draw()
{
	for (auto e : _enemies)
	{
		e->Draw();

	}
}