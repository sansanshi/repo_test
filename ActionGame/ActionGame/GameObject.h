#pragma once
#include"Collider.h"
#include<memory>

#define GRAVITY 3.0f

class GameObject
{
private:

protected:
	Collider _collider;
	bool _isAvailable;
	//bool _isCollidable;
public:
	GameObject();
	~GameObject();

	//virtual void OnCollided(Collider*)=0;
	virtual void OnCollided(GameObject*)=0;
	virtual void OnCollided(std::shared_ptr<GameObject>) = 0;

	Collider& GetCollider(){ return _collider; }
	ColliderType ColType(){ return _collider.Type(); }
	CharacterType CharaType(){ return _collider.CharaType(); }
	bool IsAvailable(){ return _isAvailable; }


	virtual void Reject(Vector2)=0;//âüÇµñﬂÇµä÷êî
};

