#include "BlockFactory.h"
#include"Player.h"
#include"Block.h"
#include"MovableBlock.h"
#include<Dxlib.h>


BlockFactory::BlockFactory(Player& player, Camera& camera) :_player(player), _cameraRef(camera)
{
	_imgMap[bt_movable] = LoadGraph("img/movableBlock.png");
	_imgMap[bt_vmovable] = LoadGraph("img/vmovableBlock.png");
	_imgMap[bt_normal] = LoadGraph("img/normalBlock.png");
}


BlockFactory::~BlockFactory()
{
	for (auto& h : _imgMap){//イメージハンドルの削除
		DeleteGraph(h.second);
	}
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
		ret.reset(new MovableBlock(pos,_imgMap[bt],_cameraRef));
		_blocks.push_back(ret);
		break;
	case bt_vmovable:
		break;
	default:
		break;
	}
}

void
BlockFactory::Draw()
{
	std::vector<std::shared_ptr<Block>>::iterator it = _blocks.begin();
	for (; it != _blocks.end();)
	{
		(*it)->Draw();
		++it;
	}
}

void
BlockFactory::Update()
{
	std::vector<std::shared_ptr<Block>>::iterator it = _blocks.begin();
	for (; it != _blocks.end();)
	{
		(*it)->Update();
		++it;
	}
}