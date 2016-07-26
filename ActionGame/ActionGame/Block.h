#pragma once
#include "Geometry.h"
#include"Collider.h"
enum BlockType{
	bt_none,
	bt_normal,//普通のブロック
	bt_movable,//動くブロック
	bt_vmovable,//動くブロック（縦方向)
	bt_slanting,


};
class Player;
class Block
{
private:

protected:
	Vector2 _pos;
	Collider _collider;
	int _handle;

	BlockType _blockType;

public:
	Block();
	~Block();
	Collider& GetCollider(){ return _collider; }

	virtual void Update() = 0;

	virtual void OnCollided(Player& player)=0;
	//virtual void OnCollided(GameObject* obj) = 0;
	virtual void Draw() = 0;
	virtual void DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag)=0;

	BlockType GetBlockType(){ return _blockType; }
	

};

