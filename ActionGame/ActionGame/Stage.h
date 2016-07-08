#pragma once
#include "GameObject.h"

class Camera;
class Stage :
	public GameObject
{
private:
	Vector2 _pos;
	int _handle;
	Camera& _cameraRef;

public:
	Stage(Camera&);
	~Stage();

	void OnCollided(Collider*);
	void OnCollided(GameObject*);
	void OnCollided(std::shared_ptr<GameObject>);

	void Draw();
	void DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag);
};
