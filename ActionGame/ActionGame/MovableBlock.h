#pragma once
#include "Block.h"
class Camera;
class MovableBlock :
	public Block
{
private:
	Vector2 _vel;
	int _timer;
	int n;
	Camera& _cameraRef;
	
public:
	MovableBlock(Vector2 pos,int handle,Camera& camera);
	~MovableBlock();

	void OnCollided(Player& player);
	void Update();
	void Draw();
	void DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag);
};

