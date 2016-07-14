#include "NormalBlock.h"
#include"Camera.h"
#include<DxLib.h>

NormalBlock::NormalBlock(Camera& camera) :_cameraRef(camera)
{
}


NormalBlock::~NormalBlock()
{
}



void
NormalBlock::Update()
{

}

void
NormalBlock::OnCollided(Player& player)
{

}

void
NormalBlock::Draw()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 32, 32, 16, 16, 1.0f, 0.0, _handle, false, false);
}
void
NormalBlock::DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag)
{
	DrawRectRotaGraph2(x + _cameraRef.OffsetX(), y, srcX, srcY, width, height, cx, cy, extRate, angle, handle, transFlag, turnFlag);
}

