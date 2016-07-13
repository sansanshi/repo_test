#pragma once
#include "Block.h"
class MovableBlock :
	public Block
{
private:
	Vector2 _vel;
	int _timer;
	int n;
public:
	MovableBlock();
	MovableBlock(Vector2 pos,int handle);
	~MovableBlock();

	void OnCollided(Player& player);
	void Update();
};

