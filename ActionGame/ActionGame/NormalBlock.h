#pragma once
#include "Block.h"
class Camera;
class Player;
class NormalBlock :
	public Block
{
private:
	Camera& _cameraRef;
public:
	NormalBlock(Camera& camera);
	~NormalBlock();
	void Update();
	void Draw();
	void OnCollided(Player&);
	void DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag);
};

