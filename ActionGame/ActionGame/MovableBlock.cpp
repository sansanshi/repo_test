#include "MovableBlock.h"
#include"Player.h"

MovableBlock::MovableBlock()
{
}
MovableBlock::MovableBlock(Vector2 pos, int handle) :_timer(0), n(0)
{
	_pos = pos;
	_handle = handle;
	_rc.width = 32;
	_rc.height = 32;
	_rc.SetCenter(_pos);
	_vel.Init();
	_vel.x = 3.0f;
}


MovableBlock::~MovableBlock()
{
}

void
MovableBlock::OnCollided(Player& player)
{
	//player.Move(_vel);
}
void
MovableBlock::Update()
{
	++_timer;
	n = (n + 1 )%(30*2);
	int m = abs(n - 30);
	_vel.x = m;

	_pos += _vel;
	_rc.SetCenter(_pos);
}