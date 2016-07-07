#pragma once
#include<memory>
#include"Geometry.h"
#include<vector>

enum BlockType{
	bt_none,
	bt_normal,//普通のブロック
	bt_movable,//動くブロック
	bt_vmovable,//動くブロック（縦方向)


};
class Block;
class BlockFactory
{
private:
	std::vector<Rect> _blocks;
public:
	BlockFactory();
	~BlockFactory();
	void CreateBlock(BlockType,Vector2);
	void Update();

	std::vector<Rect>& GetBlocks(){ return _blocks; }
};

