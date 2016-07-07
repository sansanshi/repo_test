#pragma once
#include "Geometry.h"
class Player;
class Block
{
	Rect _rc;

public:
	Block();
	~Block();
	Rect& GetRect();

	virtual void OnCollided(Player& player)=0;
};

