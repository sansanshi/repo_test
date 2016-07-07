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
	
public:
	Enemy();
	~Enemy();
	virtual void Update()=0;
	virtual void Draw()=0;


	virtual void OnCollided(GameObject*) = 0;
	virtual void OnCollided(Collider*) = 0;
	virtual void OnCollided(std::shared_ptr<GameObject>)=0;
	virtual void Kill() = 0;
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

