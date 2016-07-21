#pragma once
#include<memory>
#include"Geometry.h"
#include<vector>
#include<map>

#include"Block.h"
class Block;
class Player;
class Camera;
class BlockFactory
{
private:
	std::vector<std::shared_ptr<Block>> _blocks;
	std::map<BlockType, int> _imgMap;
	Player& _player;
	Camera& _cameraRef;
public:
	BlockFactory(Player&,Camera&);
	~BlockFactory();
	void CreateBlock(BlockType bt,Vector2 pos);
	void Update();

	std::vector<std::shared_ptr<Block>>& GetBlocks(){ return _blocks; }
	void Draw();
};

