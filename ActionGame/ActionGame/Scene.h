#pragma once
class Scene
{
protected:
	static bool _isPressedEnter;
public:
	Scene();
	~Scene();

	virtual void Update() = 0;
};

