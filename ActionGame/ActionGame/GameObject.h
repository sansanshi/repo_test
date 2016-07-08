#pragma once
#include"Collider.h"
#include<memory>
class GameObject
{
private:

protected:
	Collider _collider;
	bool _isAvailable;
	bool _isCollidable;
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
	bool IsCollidable(){ return _isCollidable; }
};
