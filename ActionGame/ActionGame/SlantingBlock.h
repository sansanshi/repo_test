#pragma once
#include "Block.h"
class Camera;
class SlantingBlock :
	public Block
{
private:
	Camera& _cameraRef;
	float _angle;
	
	
public:
	void Update();
	void Draw();
	void DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag);
	SlantingBlock(Vector2,Camera&);
	~SlantingBlock();
};

