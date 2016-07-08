#pragma once
#include "Enemy.h"
class BatMan :
	public Enemy
{
public:
	BatMan();
	~BatMan();
	void Reject(Vector2);
};

