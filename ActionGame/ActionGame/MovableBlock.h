#pragma once
#include "Block.h"
class MovableBlock :
	public Block
{
private:
	Vector2 _vel;
public:
	MovableBlock();
	~MovableBlock();

	void OnCollided(Player& player);
};

