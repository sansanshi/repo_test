#pragma once
#include"GameObject.h"
class EnemyBullet:public GameObject
{
private:
	Vector2 _pos;
	int _handle;
	Vector2 _velocity;

	bool _isLeft;

public:
	EnemyBullet(Vector2 pos,Vector2 velocity,int handle);
	~EnemyBullet();
	void Update();

	void OnCollided(Collider*);
	void OnCollided(GameObject*);
	void OnCollided(std::shared_ptr<GameObject>);

	void Draw();
};

/*std::string str;
str[0]='a'
str[1]='b'

&str[0]とかで表示させようとすると、このstrには終端記号が無いのでどこまでも表示し続けようとする

一文字ずつ表示させたいならclearString.substr(0,1).c_str
文字に影付けたかったらちょっとずらした位置に真っ黒で同じ文字を表示する
*/
