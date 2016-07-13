#pragma once
#include "Geometry.h"
class Player;
class Block
{
	Rect _rc;

public:
	Block();
	Block(BlockType, Vector2);
	~Block();
	Rect& GetRect();

	virtual void OnCollided(Player& player)=0;
};

