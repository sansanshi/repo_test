#include "Camera.h"
#include<DxLib.h>
#include"Player.h"


Camera::Camera(Player& playerRef):_playerRef(playerRef)
{
}


Camera::~Camera()
{
	int a = 0;
}


void
Camera::SetUp()
{
	int w, h, depth;
	DxLib::GetScreenState(&w, &h, &depth);
	_rc.width = w;
	_rc.height = h;
	_rc.SetCenter(_playerRef.GetCenter()+_quakeOffset);

	_stageWidth = w;
	//_offsetX = OffsetX();
}

float 
Camera::OffsetX()
{
	return -_playerRef.GetCenter().x + _rc.width / 2+_quakeOffset.x;//return _rc.Left();
}

//void
//Camera::DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag){
//	DrawRectRotaGraph2(x + _offsetX, y, srcX, srcY, width, height, cx, cy, extRate, angle, handle, transFlag, turnFlag);
//}

void
Camera::InvokeQuake(float power)
{
	_quakeFrame = 60;
	_quakeOffset = Vector2(power, 0);
}
void 
Camera::Update()
{
	if (_quakeFrame > 0)
	{
		_quakeOffset.x = -_quakeOffset.x*0.9f;
		--_quakeFrame;
	}
	else
	{
		_quakeOffset = Vector2(0, 0);
	}
}