#include "Stage.h"
#include"Camera.h"
#include<DxLib.h>

Stage::Stage(Camera& camera) :_cameraRef(camera)
{
	_collider = Collider(this, ct_stage, col_stage);
	_collider.SetRect(Rect(_pos, 29, 128));
}


Stage::~Stage()
{
}

void
Stage::OnCollided(Collider* col)
{

}

void
Stage::OnCollided(GameObject* obj)
{

}

void
Stage::OnCollided(std::shared_ptr<GameObject> obj)
{

}

void 
Stage::Draw()
{

}

void
Stage::DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag)
{
	DrawRectRotaGraph2(x + _cameraRef.OffsetX(), y, srcX, srcY, width, height, cx, cy, extRate, angle, handle, transFlag, turnFlag);
}