#include "MovableBlock.h"
#include"Player.h"
#include<DxLib.h>
#include"Camera.h"

MovableBlock::MovableBlock(Vector2 pos, int handle, Camera& camera) :_timer(0), n(0), _cameraRef(camera)
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

void
MovableBlock::Draw()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 32, 32, 16, 16, 1.0f, 0.0, _handle, false, false);
}
void
MovableBlock::DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag)
{
	DrawRectRotaGraph2(x + _cameraRef.OffsetX(), y, srcX, srcY, width, height, cx, cy, extRate, angle, handle, transFlag, turnFlag);
}

