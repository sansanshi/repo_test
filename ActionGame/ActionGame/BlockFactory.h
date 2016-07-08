#pragma once
#include<memory>
#include"Geometry.h"
#include<vector>
#include<map>

enum BlockType{
	bt_none,
	bt_normal,//普通のブロック
	bt_movable,//動くブロック
	bt_vmovable,//動くブロック（縦方向)


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

