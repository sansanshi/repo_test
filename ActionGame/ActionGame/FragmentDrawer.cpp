#include "FragmentDrawer.h"
#include <math.h>
#include<assert.h>

Fragment::Fragment() {

	_headVert = NULL;
	_velocity.x = 0.0f;
	_velocity.y = -3.0f - 0.1f*static_cast<float>(rand() % 20 - 10);
	_center.x = 0.f;
	_center.y = 0.f;
	_accel.x = 0.04f*static_cast<float>(rand() % 21 - 10) ;//rand()%20-10=-10~9
	_accel.y = 0.3f;
	_angleVel = 0.0015f*static_cast<float>(rand() % 20 - 10);
	_fallTimer = 0;
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
	Vector2 temp[3];
	++_fallTimer;

	_fallTimer>5 ? _velocity.x += _accel.x:0;
	_fallTimer > 5 ? _velocity.y += _accel.y:0;
	//まず回転(重心中心)
	//平行移動に始まり平行移動に終わる()
	for (int i = 0; i<3; ++i){
		DxLib::VERTEX& vert = *(_headVert + i);//このvertはinput/outputみたいな扱いにして下のvを動かして最後にvert=vとかしてみる//ここに入ってきた時のvertは拡大されてるはず
		DxLib::VERTEX v = identityVert[i];//*(_headVert+i);

		vert.x -= _center.x;//原点中心になるように平行移動
		vert.y -= _center.y;
		//v.x -= _center.x;
		//v.y -= _center.y;
		//この辺でスケールを1に出来ればいける
		//元の状態のvertの情報がほしい←identityVertに取れたっぽい
		
		//コブラのマシンはサイコガン
		//v.x = v.x*5.0f + v.y*0.f;//拡大計算//毎フレーム拡大がかかってるっぽい
		//v.y = v.x*0.0f + v.y*5.0f;
		float x = vert.x*cos(_angleVel) - vert.y*sin(_angleVel);//回転計算
		float y = vert.x*sin(_angleVel) + vert.y*cos(_angleVel);//ここでv.x使ってるのに上でv.x=~~ってしてるのおかしい
		//v.x *= 2.0f;
		//v.y *= 2.0f;
		//v.x += _center.x;
		//v.y += _center.y;
		
		vert.x = x;//計算によって求めた回転後座標に移動
		vert.y = y;
		//vert = v;
		vert.x *= 0.985f;//少しずつ小さくしてる？
		vert.y *= 0.985f;
		vert.x += _center.x;//原点中心にしてたので元の位置に戻す
		vert.y += _center.y;
		
		//vert.x = expansionVert[i].x + x;
		//vert.y = expansionVert[i].y + y;

		//落下
		_fallTimer>3 ? vert.x += _velocity.x : 0 ;
		_fallTimer>3 ? vert.y += _velocity.y : 0 ;
		//vert.x = 200 + expansionVert[i].x;//expansionVertは多分いらない ここに入ってきたときのvertはexpansionVertと同じ
		//vert.y = 200 + expansionVert[i].y;

		//透明度UP
		vert.a -= 1;

		temp[i].x = vert.x;
		temp[i].y = vert.y;

	}
	CalculateCenter();

	for (int i = 0; i < 3; i++){
		//DrawLine(temp[i].x, temp[i].y, temp[(i + 1)%3].x, temp[(i + 1)%3].y, 0xffffff);
	}
	//for (int i = 0; i < 3; i++){//ここはできてるっぽい
	//	DxLib::VERTEX v = identityVert[i];
	//	DxLib::VERTEX v_ = identityVert[(i+1)%3];
	//	DrawLine(v.x, v.y, v_.x, v_.y, 0xffffff);
	//}
}
void
Fragment::Scalling(float size)//毎フレームこれ呼んでると動かない（中で座標を弄ってるので
{//この中でidentityVertを使って計算してるので前のTranslationの結果が反映されてない
	Vector2 temp[3];

	_velocity.x += _accel.x;
	_velocity.y += _accel.y;
	//まず回転(重心中心)
	//平行移動に始まり平行移動に終わる()
	for (int i = 0; i<3; ++i){
		DxLib::VERTEX& vert = *(_headVert + i);//このvertはinput/outputみたいな扱いにして下のvを動かして最後にvert=vとかしてみる
		DxLib::VERTEX v = identityVert[i];//*(_headVert+i);

		//vert.x -= _center.x;//原点中心になるように平行移動
		//vert.y -= _center.y;
		v.x -= _center.x;
		v.y -= _center.y;
		//この辺でスケールを1に出来ればいける
		//元の状態のvertの情報がほしい←identityVertに取れたっぽい

		//コブラのマシンはサイコガン
		//float tempx = identityVert[i].x - _center.x;
		//float tempy = identityVert[i].y - _center.y;
		float x = v.x*size + v.y*0.f;//拡大計算//毎フレーム拡大がかかってるっぽい
		float y = v.x*0.0f + v.y*size;
		
		x += _center.x*size;//ここで詰まってた　拡大後には中心座標*拡大倍率を＋する
		y += _center.y*size;

		vert.x = x;//計算によって求めた回転後座標に移動
		vert.y = y;
		//vert = v;
		//vert.x *= 0.985f;//少しずつ小さくしてる？
		//vert.y *= 0.985f;
		//vert.x += _center.x;//原点中心にしてたので元の位置に戻す
		//vert.y += _center.y;
		expansionVert[i] = vert;
		

		temp[i].x = vert.x;
		temp[i].y = vert.y;

	}
	CalculateCenter();

	/*for (int i = 0; i < 3; i++){
	DrawLine(temp[i].x, temp[i].y, temp[(i + 1)%3].x, temp[(i + 1)%3].y, 0xffffff);
	}*/
	//for (int i = 0; i < 3; i++){//ここはできてるっぽい
	//	DxLib::VERTEX v = identityVert[i];
	//	DxLib::VERTEX v_ = identityVert[(i + 1) % 3];
	//	DrawLine(v.x, v.y, v_.x, v_.y, 0xffffff);
	//}
}

FragmentDrawer::FragmentDrawer(const char* filePath) : BaseDrawer(filePath),//ファイルパス貰ったらその画像でバラバラにするための画像を作成（_capHandleに入れる）
_breaking(false),
_capHandle(0)
{
	SetUseDivGraphFlag(FALSE);
	CreateVertices(8, 6, 640, 480);
	_capHandle = MakeGraph(640, 480);
}
FragmentDrawer::FragmentDrawer(int handle) : BaseDrawer(handle),//ファイルパス貰ったらその画像でバラバラにするための画像を作成（_capHandleに入れる）
_breaking(false),
_capHandle(0)
{
	SetUseDivGraphFlag(FALSE);
	int sizeX, sizeY;
	GetGraphSize(handle, &sizeX, &sizeY);
	CreateVertices(3, 4, sizeX, sizeY);
	_capHandle = handle;
}

FragmentDrawer::FragmentDrawer(void) : BaseDrawer(), _breaking(false), _capHandle(0)//これどこかから呼ばれてるか分からん
{//→解決　GrabmanのコンストラクタでFragmentDrawerの実体を定義してるのでGrabmanが作られた時点でFragmentDrawerのデフォルトコンストラクタが走る
	SetUseDivGraphFlag(FALSE);
	CreateVertices(8, 6, 640, 480);
	_capHandle = MakeGraph(640, 480);
}//Grabmanのコンストラクタで初期化子を使うかポインタ型で定義することで解決



FragmentDrawer::~FragmentDrawer(void)
{
	SetUseDivGraphFlag(TRUE);
}

void
FragmentDrawer::Break(){
	_breaking = true;
}
void
FragmentDrawer::Break(Vector2 power)
{
	_breaking = true;
	std::vector<Fragment>::iterator it = _fragments.begin();
	for (; it != _fragments.end();)
	{
		it->AddPower(power);
		it++;
	}
}
void
Fragment::AddPower(Vector2 power)
{
	_velocity += power;
}

void
FragmentDrawer::Capture(){
	int r = DxLib::GetDrawScreenGraph(0, 0, 640, 480, _capHandle);
}
void
FragmentDrawer::CreateGraph(int handle)//一応作ったけどコンストラクタで_capHandle設定してるから使わないかも
{
	_capHandle = handle;
}

void
FragmentDrawer::Draw(){

	//破片描画
	if (_breaking){
		DxLib::DrawPolygon(&_vertices[0], _vertices.size() / 3, _capHandle, true, true);
		std::vector<Fragment>::iterator it = _fragments.begin();
		for (; it != _fragments.end(); ++it){
			it->Fall();
			//it->Scalling();
		}

	}
	else{
		BaseDrawer::Draw();
	}
}


void
FragmentDrawer::CreateVertices(int divx, int divy, int screenW, int screenH){
	_fragments.resize(divx*divy * 2);//四角形→三角形に分割してるのでfragment（三角形の欠片？）の数は分割数（四角形の数）*2
	_vertices.resize(divx*divy * 6);//四角形が2つの三角形になった→四角形1個で頂点6つになっているので*6
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
			_vertices[headidx + 0].g = _vertices[headidx + 0].b = 0;//試しに頂点カラーを赤にしてみる
			_vertices[headidx + 0].x = left; _vertices[headidx + 0].y = top;
			_vertices[headidx + 0].u = leftu; _vertices[headidx + 0].v = topv;
			_vertices[headidx + 1].r = _vertices[headidx + 1].g = _vertices[headidx + 1].b = _vertices[headidx + 1].a = 255;
			//_vertices[headidx + 1].g = _vertices[headidx + 1].b = 0;
			_vertices[headidx + 1].x = right; _vertices[headidx + 1].y = top;
			_vertices[headidx + 1].u = rightu; _vertices[headidx + 1].v = topv;
			_vertices[headidx + 2].r = _vertices[headidx + 2].g = _vertices[headidx + 2].b = _vertices[headidx + 2].a = 255;
			//_vertices[headidx + 2].g = _vertices[headidx + 2].b = 0;
			_vertices[headidx + 2].x = left; _vertices[headidx + 2].y = bottom;
			_vertices[headidx + 2].u = leftu; _vertices[headidx + 2].v = bottomv;
			int fragHeadIdx = (i + divx*j) * 2;//上半分→下半分で、1つ横の上半分は＋1ではないので＊2
			_fragments[fragHeadIdx]._headVert = &_vertices[headidx + 0];
			_fragments[fragHeadIdx].CalculateCenter();
			for (int i = 0; i < 3; i++)//何もしてない頂点情報を保存テスト
			{
				_fragments[fragHeadIdx].identityVert[i] = _vertices[headidx + 0 + i];
			}

			//下半分
			_vertices[headidx + 3].r = _vertices[headidx + 3].g = _vertices[headidx + 3].b = _vertices[headidx + 3].a = 255;
			_vertices[headidx + 3].g = _vertices[headidx + 3].b = 0;//頂点カラーを赤に
			_vertices[headidx + 3].x = right; _vertices[headidx + 3].y = top;
			_vertices[headidx + 3].u = rightu; _vertices[headidx + 3].v = topv;
			_vertices[headidx + 4].r = _vertices[headidx + 4].g = _vertices[headidx + 4].b = _vertices[headidx + 4].a = 255;
			_vertices[headidx + 4].x = left; _vertices[headidx + 4].y = bottom;
			_vertices[headidx + 4].u = leftu; _vertices[headidx + 4].v = bottomv;
			_vertices[headidx + 5].r = _vertices[headidx + 5].g = _vertices[headidx + 5].b = _vertices[headidx + 5].a = 255;
			_vertices[headidx + 5].g = _vertices[headidx + 5].b = 0;//頂点カラーを赤に
			_vertices[headidx + 5].x = right; _vertices[headidx + 5].y = bottom;
			_vertices[headidx + 5].u = rightu; _vertices[headidx + 5].v = bottomv;
			_fragments[fragHeadIdx + 1]._headVert = &_vertices[headidx + 3];
			_fragments[fragHeadIdx + 1].CalculateCenter();
			for (int i = 0; i < 3; i++)//何もしてない頂点情報を保存テスト
			{
				_fragments[fragHeadIdx+1].identityVert[i] = _vertices[headidx + 3 + i];
			}

		}
	}
}

void
FragmentDrawer::FragmentScalling(float size)
{
	std::vector<Fragment>::iterator it = _fragments.begin();
	for (; it != _fragments.end();)
	{
		it->Scalling(size);
		it++;
	}
}

void
FragmentDrawer::TurnUV()
{
	std::vector<DxLib::VERTEX>::iterator it = _vertices.begin();
	for (; it != _vertices.end();)
	{
		it->u = static_cast<float>(1.0f - it->u);
		it++;
	}
}

void
FragmentDrawer::SetPos(Vector2 pos)
{
	std::vector<Fragment>::iterator it = _fragments.begin();
	for (; it != _fragments.end();)
	{
		it->SetCenter(pos);
		it++;
	}
}

void
Fragment::SetCenter(Vector2 pos)
{
	_center += pos;
}

void 
FragmentDrawer::FragmentTranslation(Vector2 vec)
{
	std::vector<Fragment>::iterator it = _fragments.begin();
	for (; it != _fragments.end();)
	{
		it->Translation(vec);
		it++;
	}
}

void
Fragment::Translation(Vector2 pos)
{
	for (int i = 0; i<3; ++i){
		DxLib::VERTEX& vert = *(_headVert + i);//このvertはinput/outputみたいな扱いにして下のvを動かして最後にvert=vとかしてみる//ここに入ってきた時のvertは拡大されてるはず
		//DxLib::VERTEX v = identityVert[i];//*(_headVert+i);

		vert.x -= _center.x;//posの位置に座標を設定する関数なので一旦中心を0,0に持っていく
		vert.y -= _center.y;
		
		
		vert.x += pos.x+_center.x;//原点中心にしてたので元の位置に戻す
		vert.y += pos.y + _center.y;

	
	}
	CalculateCenter();
	Vector2 temp;
	temp = _center;
	int a;
	
}