#include "VMovableBlock.h"
#include"Player.h"
#include<DxLib.h>
#include"Camera.h"

VMovableBlock::VMovableBlock(Vector2 pos, int& handle, Camera& camera) :_timer(0), n(0), _cameraRef(camera)
{
	_pos = pos;
	_handle = handle;
	_collider.width = 32;
	_collider.height = 32;
	_collider.SetCenter(_pos);
	_vel.Init();
	//_vel.x = 3.0f;
	_blockType = bt_vmovable;
}


VMovableBlock::~VMovableBlock()
{
}

void
VMovableBlock::OnCollided(Player& player)
{
	player.Move(_vel);
}
void
VMovableBlock::Update()
{
	++_timer;
	n = (n + 1) % (30 * 2);
	int m = abs(n - 30);
	_vel.y = m - 15;
	_vel = _vel*0.5f;

	_pos += _vel;
	_collider.SetCenter_Cam(_pos,_cameraRef.OffsetX());
}

void
VMovableBlock::Draw()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 32, 32, 16, 16, 1.0f, 0.0, _handle, false, false);
}
void
VMovableBlock::DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag)
{
	DrawRectRotaGraph2(x + _cameraRef.OffsetX(), y, srcX, srcY, width, height, cx, cy, extRate, angle, handle, transFlag, turnFlag);
}

