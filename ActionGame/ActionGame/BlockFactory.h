#pragma once
#include<memory>
#include"Geometry.h"
#include<vector>

enum BlockType{
	bt_none,
	bt_normal,//���ʂ̃u���b�N
	bt_movable,//�����u���b�N
	bt_vmovable,//�����u���b�N�i�c����)


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

