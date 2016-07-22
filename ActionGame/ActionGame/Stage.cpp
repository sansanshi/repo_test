#include "Stage.h"
#include"Camera.h"
#include<DxLib.h>

Stage::Stage(Camera& camera) :_cameraRef(camera)
{
	_pos = Vector2(0, 430);
	_collider = Collider(this, ct_stage, col_stage);
	_collider.SetRect(Rect(_pos, 1920, 50));
	_collider.SetCenter_Cam(_pos, _cameraRef.OffsetX());
	
	
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

	_collider.Draw();
}

void
Stage::DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag)
{
	DrawRectRotaGraph2(x + _cameraRef.OffsetX(), y, srcX, srcY, width, height, cx, cy, extRate, angle, handle, transFlag, turnFlag);
}

void 
Stage::Reject(Vector2 vec)
{

}

void
Stage::Update()
{
	//_pos.x = _cameraRef.OffsetX();//この行必要ない？
	n = (n + 1) % (30 * 2);
	int m = abs(n - 30);
	//_vel.y = m - 15;

	_pos += _vel*0.05f;
	_collider.SetCenter_Cam(_pos, _cameraRef.OffsetX());
}