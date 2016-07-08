#pragma once
#include<memory>
#include"Geometry.h"
#include<vector>
#include<map>

enum BlockType{
	bt_none,
	bt_normal,//���ʂ̃u���b�N
	bt_movable,//�����u���b�N
	bt_vmovable,//�����u���b�N�i�c����)


};
class Block;
class Player;
class BlockFactory
{
private:
	std::vector<std::shared_ptr<Block>> _blocks;
	std::map<BlockType, int> _imgMap;
	Player& _player;
public:
	BlockFactory();
	~BlockFactory();
	void CreateBlock(BlockType bt,Vector2 pos);
	void Update();

	std::vector<std::shared_ptr<Block>>& GetBlocks(){ return _blocks; }
};

