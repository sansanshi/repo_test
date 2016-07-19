#pragma once
#include "Collider.h"
#include"GameObject.h"
#include<memory>
#include<vector>
class Enemy :
	public GameObject
{

private:

protected:
	Vector2 _pos;
	Vector2 _velocity;
	Collider _attackCol;
	
public:
	Enemy();
	~Enemy();
	virtual void Update()=0;
	virtual void Draw()=0;


	virtual void OnCollided(GameObject*) = 0;
	virtual void OnCollided(Collider*) = 0;
	virtual void OnCollided(std::shared_ptr<GameObject>)=0;
	virtual void Kill() = 0;
	Vector2 GetPos(){ return _pos; }
	Collider GetAttackCol(){ return _attackCol; }
	//void virtual 
};
//class A{
//	A(){};
//	~A(){};
//	void Func(){};
//};
//
//int main(){ 
//	std::vector<std::shared_ptr<A>> v;
//	{
//		std::shared_ptr<A> b(new A());
//		{
//			std::shared_ptr<A> p(new A());
//			p->Func();
//			b = p;
//		}
//		v.push_back(b);
//	}
//}

