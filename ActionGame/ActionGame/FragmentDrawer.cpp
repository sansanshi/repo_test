#include "FragmentDrawer.h"
#include <math.h>
#include<assert.h>

Fragment::Fragment() {

	_headVert = NULL;
	_velocity.x = 1.5f;
	_velocity.y = -3.0f - 0.1f*static_cast<float>(rand() % 20 - 10);
	_center.x = 0.f;
	_center.y = 0.f;
	_accel.x = 0.04f*static_cast<float>(rand() % 20 - 10) - 0.1f;
	_accel.y = 0.3f;
	_angleVel = 0.015f*static_cast<float>(rand() % 20 - 10);
}

Fragment::~Fragment()
{

}

void
Fragment::CalculateCenter(){
	this->_center.x = (_headVert->x + (_headVert + 1)->x + (_headVert + 2)->x) / 3.f;//3点の平均を取って中心を求めてる
	this->_center.y = (_headVert->y + (_headVert + 1)->y + (_headVert + 2)->y) / 3.f;
}
void
Fragment::Fall(){

	_velocity.x += _accel.x;
	_velocity.y += _accel.y;
	//まず回転(重心中心)
	//平行移動に始まり平行移動に終わる()
	for (int i = 0; i<3; ++i){
		DxLib::VERTEX& vert = *(_headVert + i);
		vert.x -= _center.x;//原点中心になるように平行移動
		vert.y -= _center.y;
		//コブラのマシンはサイコガン

		float x = vert.x*cos(_angleVel) - vert.y*sin(_angleVel);//回転計算
		float y = vert.x*sin(_angleVel) + vert.y*cos(_angleVel);
		vert.x = x;//計算によって求めた回転後座標に移動
		vert.y = y;
		vert.x *= 0.985f;//少しずつ小さくしてる？
		vert.y *= 0.985f;
		vert.x += _center.x;//原点中心にしてたので元の位置に戻す
		vert.y += _center.y;

		//落下
		vert.x += _velocity.x;
		vert.y += _velocity.y;

		//透明度UP
		vert.a -= 1;

	}
	CalculateCenter();


}

FragmentDrawer::FragmentDrawer(const char* filePath) : BaseDrawer(filePath),
_breaking(false),
_capHandle(0)
{
	SetUseDivGraphFlag(FALSE);
	CreateVertices(8, 6, 640, 480);
	_capHandle = MakeGraph(640, 480);
}

FragmentDrawer::FragmentDrawer(void) : BaseDrawer(),
_breaking(false),
_capHandle(0)
{
	SetUseDivGraphFlag(FALSE);
	CreateVertices(8, 6, 640, 480);
	_capHandle = MakeGraph(640, 480);
}



FragmentDrawer::~FragmentDrawer(void)
{
	SetUseDivGraphFlag(TRUE);
}

void
FragmentDrawer::Break(){
	_breaking = true;
}

void
FragmentDrawer::Capture(){
	int r = DxLib::GetDrawScreenGraph(0, 0, 640, 480, _capHandle);
}

void
FragmentDrawer::Draw(){

	//破片描画
	if (_breaking){
		DxLib::DrawPolygon(&_vertices[0], _vertices.size() / 3, _capHandle, true, true);
		std::vector<Fragment>::iterator it = _fragments.begin();
		for (; it != _fragments.end(); ++it){
			it->Fall();
		}

	}
	else{
		BaseDrawer::Draw();
	}
}


void
FragmentDrawer::CreateVertices(int divx, int divy, int screenW, int screenH){
	_fragments.resize(divx*divy * 2);//四角形→三角形に分割してるので*2
	_vertices.resize(divx*divy * 6);//2つの三角形になった→四角形1個で頂点6つになっているので*6
	for (int j = 0; j<divy; ++j){
		float top = (screenH / divy)*j;
		float bottom = (screenH / divy)*(j + 1);
		float topv = (1.f / divy)*static_cast<float>(j);
		float bottomv = (1.f / divy)*static_cast<float>(j + 1);
		for (int i = 0; i<divx; ++i){
			float left = (screenW / divx)*static_cast<float>(i);
			float right = (screenW / divx)*static_cast<float>(i + 1);
			float leftu = (1.f / divx)*static_cast<float>(i);
			float rightu = (1.f / divx)*static_cast<float>(i + 1);

			int headidx = (i + divx*j) * 6;

			//上半分
			_vertices[headidx + 0].r = _vertices[headidx + 0].g = _vertices[headidx + 0].b = _vertices[headidx + 0].a = 255;
			_vertices[headidx + 0].x = left; _vertices[headidx + 0].y = top;
			_vertices[headidx + 0].u = leftu; _vertices[headidx + 0].v = topv;
			_vertices[headidx + 1].r = _vertices[headidx + 1].g = _vertices[headidx + 1].b = _vertices[headidx + 1].a = 255;
			_vertices[headidx + 1].x = right; _vertices[headidx + 1].y = top;
			_vertices[headidx + 1].u = rightu; _vertices[headidx + 1].v = topv;
			_vertices[headidx + 2].r = _vertices[headidx + 2].g = _vertices[headidx + 2].b = _vertices[headidx + 2].a = 255;
			_vertices[headidx + 2].x = left; _vertices[headidx + 2].y = bottom;
			_vertices[headidx + 2].u = leftu; _vertices[headidx + 2].v = bottomv;
			int fragHeadIdx = (i + divx*j) * 2;
			_fragments[fragHeadIdx]._headVert = &_vertices[headidx + 0];
			_fragments[fragHeadIdx].CalculateCenter();

			//下半分
			_vertices[headidx + 3].r = _vertices[headidx + 3].g = _vertices[headidx + 3].b = _vertices[headidx + 3].a = 255;
			_vertices[headidx + 3].x = right; _vertices[headidx + 3].y = top;
			_vertices[headidx + 3].u = rightu; _vertices[headidx + 3].v = topv;
			_vertices[headidx + 4].r = _vertices[headidx + 4].g = _vertices[headidx + 4].b = _vertices[headidx + 4].a = 255;
			_vertices[headidx + 4].x = left; _vertices[headidx + 4].y = bottom;
			_vertices[headidx + 4].u = leftu; _vertices[headidx + 4].v = bottomv;
			_vertices[headidx + 5].r = _vertices[headidx + 5].g = _vertices[headidx + 5].b = _vertices[headidx + 5].a = 255;
			_vertices[headidx + 5].x = right; _vertices[headidx + 5].y = bottom;
			_vertices[headidx + 5].u = rightu; _vertices[headidx + 5].v = bottomv;
			_fragments[fragHeadIdx + 1]._headVert = &_vertices[headidx + 3];
			_fragments[fragHeadIdx + 1].CalculateCenter();
		}
	}
}