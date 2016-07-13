#include "BlockFactory.h"
#include"Player.h"
#include"Block.h"
#include"MovableBlock.h"
#include<Dxlib.h>


BlockFactory::BlockFactory(Player& player) :_player(player)
{
	_imgMap[bt_movable] = LoadGraph("img/movableBlock.png");
}


BlockFactory::~BlockFactory()
{
}

void
BlockFactory::CreateBlock(BlockType bt, Vector2 pos)
{
	std::shared_ptr<Block> ret;
	switch (bt)
	{
	case bt_none:
		break;
	case bt_normal:
		//ret.reset(new NormalBlock(bt, pos));
		break;
	case bt_movable:
		ret.reset(new MovableBlock());
		break;
	case bt_vmovable:
		break;
	default:
		break;
	}
}
