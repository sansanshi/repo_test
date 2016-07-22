#pragma once
#include"Geometry.h"
class Camera;
class Effect
{
private:
	int _handle;
	Vector2 _pos;
	int _timer;
	Camera& _cameraRef;
	bool _isAvailable;

public:
	Effect(Vector2 pos,int handle,Camera& camera);
	~Effect();

	void Update();
	void Draw();
	void DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag);
	bool IsAvailable(){ return _isAvailable; }
};

