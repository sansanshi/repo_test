#pragma once
#include"Geometry.h"
class Player;
class Camera
{
private:
	Player& _playerRef;
	Rect _rc;
	float _stageWidth;

	float _quakePower;
	Vector2 _quakeOffset;
	int _quakeFrame;
	//static float _offsetX;
public:
	Camera(Player&);
	~Camera();
	void SetUp();
	Rect& GetRect(){ return _rc; }
	float OffsetX();


	void InvokeQuake(float power);
	void Quake();
	void Update();

	//static void DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag);
};

