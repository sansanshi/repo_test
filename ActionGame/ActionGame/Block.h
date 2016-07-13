#pragma once
#include "Geometry.h"
class Player;
class Block
{
private:

protected:
	Vector2 _pos;
	Rect _rc;
	int _handle;

public:
	Block();
	~Block();
	Rect& GetRect(){ return _rc; }

	virtual void Update() = 0;

	virtual void OnCollided(Player& player)=0;
	//virtual void OnCollided(GameObject* obj) = 0;
};

