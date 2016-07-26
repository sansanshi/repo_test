#include "SlantingBlock.h"
#include"Dxlib.h"
#include"Camera.h"

#define M_PI 3.1415926536

SlantingBlock::SlantingBlock(Vector2 pos,Camera& camera) :_cameraRef(camera)
{
	_blockType = bt_slanting;
	_pos = pos;
	_collider.width = 32;
	_collider.height = 32;
	_collider.SetCenter_Cam(_pos, _cameraRef.OffsetX());
	_angle = 45*M_PI/180;
}


SlantingBlock::~SlantingBlock()
{
}

void
SlantingBlock::DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag)
{
	DrawRectRotaGraph2(x + _cameraRef.OffsetX(), y, srcX, srcY, width, height, cx, cy, extRate, angle, handle, transFlag, turnFlag);
	//DrawModiGraph
}

void
SlantingBlock::Draw()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 32, 32, 16, 16, 1.0f, 0.0, _handle, false, false);
}