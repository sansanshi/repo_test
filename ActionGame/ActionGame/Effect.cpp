#include "Effect.h"
#include<Dxlib.h>
#include"Camera.h"

Effect::Effect(Vector2 pos,int handle,Camera& camera) :_cameraRef(camera)
{
	_pos = pos;
	_handle = handle;
	_timer = 0;
	_isAvailable = true;
}


Effect::~Effect()
{
}

void
Effect::Update()
{
	_timer++;
	if (_timer > 10) _isAvailable = false;
}

void
Effect::Draw()
{
	//DrawCameraGraph(_pos.x, _pos.y, 0, 0, 8, 8, 4, 4, 3.0, 0, _handle, true, false);
	DrawRectRotaGraph2(_pos.x, _pos.y, 0, 0, 8, 8, 4, 4, 3.0, 0, _handle, true, false);
}
void
Effect::DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag){
	DrawRectRotaGraph2(x + _cameraRef.OffsetX(), y, srcX, srcY, width, height, cx, cy, extRate, angle, handle, transFlag, turnFlag);
}
