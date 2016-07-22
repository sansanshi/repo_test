#include "EnemyFactory.h"
#include<Dxlib.h>
#include"GrabMan.h"
#include"KnifeMan.h"
#include"Player.h"
#include"BatMan.h"
#include<math.h>

EnemyFactory::EnemyFactory(Player& player, Camera& camera, EnemyBulletFactory& ebulletFac) :_playerRef(player), _cameraRef(camera), _ebulletFac(ebulletFac)
{
	_enemyImgMap[et_grabman] = LoadGraph("img/grabman_.png");
	_enemyImgMap[et_knifeman] = LoadGraph("img/knifeman_.png");
	_enemyImgMap[et_batman] = LoadGraph("img/boss1_.png");
	
	_enemydeadImgMap[et_grabman] = LoadGraph("img/grabman_dead.png");
	_enemydeadImgMap[et_knifeman] = LoadGraph("img/knifeman_dead.png");
	_enemydeadImgMap[et_batman] = LoadGraph("img/boss1_dead.png");
	_timer = 0;
}



EnemyFactory::~EnemyFactory()
{
	for (auto& h : _enemyImgMap){//イメージハンドルの削除
		DeleteGraph(h.second);
	}

	//for (auto& enemy : _enemies)//シェアドポインタは誰も参照しなくなった時点で勝手に削除されるのでここはいらない
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
		ret.reset(new KnifeMan(pos, _enemyImgMap[et], _enemydeadImgMap[et], _playerRef,_cameraRef,_ebulletFac));
		_enemies.push_back(ret);
		return _enemies.back();
		break;
	case et_batman:
		ret.reset(new BatMan(pos, _enemyImgMap[et], _enemydeadImgMap[et], _playerRef, _cameraRef));
		_enemies.push_back(ret);
		return _enemies.back();
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
	if (_timer % 120 == 0){
		Vector2 offset;
		offset = _playerRef.IsRight() ? Vector2(300, 0) : Vector2(-300, 0);

		int random = std::rand();
		random %= 100;
		if (random<70)Create(et_grabman, _playerRef.GetPos() + offset);
		else if (random<90)Create(et_knifeman, _playerRef.GetPos() + offset);
		else Create(et_batman, _playerRef.GetPos() + offset);
		Vector2 pos = _playerRef.GetPos() + offset;
		int a = 0;
	}

	//更新ループ
	for (auto e : _enemies)
	{
		e->Update();
	}
	
	//不要なもの削除ループ
	_enemies.remove_if(
		[](std::shared_ptr<Enemy>& e){//ラムダ式が使われている
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