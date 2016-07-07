#pragma once


//EnemyのlistのEnemy*になってるところを全部std::shared_ptr<Enemy>にする　もともとptrなので*はつけない
//Enemyのポインタを求められるところには.getにする
//createのときEnemy＊を返していたところ(Enemy* ret=new ~~~ってしてるところをret.reset(new ~~~~)に書き換える

/*//不要なもの削除ループをこれに置き換える
_enmies.remove_if(
	[](std::shared_ptr<Enemy>& e){//ラムダ式が使われている
	return !e->IsAvailable(); 
});*/

#include<vector>
#include"Geometry.h"
#include<map>
#include<algorithm>
#include<memory>
#include<list>


class Enemy;
class Player;
class Camera;

enum EnemyType{
	et_none,
	et_grabman,
	et_knifeman,
	et_batman,
};

class EnemyFactory
{
private:
	std::list<std::shared_ptr<Enemy>> _enemies;//シェアドポインタ　自身を参照しているやつごとに参照カウンタが+1　0になる（誰も参照していない状態）と破棄される

	std::map<EnemyType, int> _enemyImgMap;
	std::map<EnemyType, int> _enemydeadImgMap;

	Player& _playerRef;
	int _timer;
	Camera& _cameraRef;

public:
	EnemyFactory(  Player& player,Camera& camera);
	//EnemyFactory();
	~EnemyFactory();
	//敵の生成
	std::shared_ptr<Enemy> Create(EnemyType et, Vector2& pos);
	std::shared_ptr<Enemy> Create(unsigned char et, Vector2& pos);
	//全体のアップデート
	void Update();
	std::list<std::shared_ptr<Enemy>>& GetEnemies();//enemiesの参照を返す
	void Draw();


};

