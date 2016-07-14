#pragma once


struct Vector2{
	float x;
	float y;
	void operator += (Vector2& obj)
	{
		x += obj.x;
		y += obj.y;
	}
	void operator -= (Vector2& obj)
	{
		x -= obj.x;
		y -= obj.y;
	}

	Vector2 operator -(const Vector2& obj) const
	{
		return Vector2(x - obj.x, y - obj.y);
	}
	Vector2 operator /(const float& f)
	{
		return Vector2(x / f, y / f);
	}
	Vector2 operator *(float f)
	{
		Vector2 r;
		r.x = x*f;
		r.y = y*f;
		return Vector2(x*f, y*f);
	}
	Vector2 operator *(int i)//intとfloatはオペレータで変換してくれないので別々に作る
	{
		return Vector2(x*i, y*i);
	}

	Vector2(float inx, float iny)
	{
		x = inx;
		y = iny;
	}
	Vector2()
	{
		x = y = 0;
	}
	bool operator ==(Vector2& v)const
	{
		return x == v.x&&y == v.y;
	}
	Vector2 operator +(const Vector2& v)const
	{
		return Vector2(x + v.x, y + v.y);
	}
	float Length();
	Vector2& Normalize();
	void Init();//Vector2(0,0)にする
};
struct Rect{
	Vector2 pos;
	//int x, y;//左上座標
	int width, height;
	int Left(){ return pos.x; };
	int Right(){ return pos.x + width; };
	int Top(){ return pos.y; };
	int Bottom(){ return pos.y + height; };

	Vector2 Center(){ return Vector2(pos.x + width / 2, pos.y + height / 2); }
	/*Rect(int inx, int iny, int inwidth, int inheight)
	{
	x = inx;
	y = iny;
	width = inwidth;
	height = inheight;
	}
	Rect()
	{
	x = y = width = height = 0;
	}*/
	Rect() :pos(0, 0), width(0), height(0){}//初期化子を使ったコンストラクタ
	Rect(float x, float y, int inw, int inh) :pos(x, y), width(inw), height(inh){}
	Rect(Vector2& inpos, int inw, int inh) :pos(inpos), width(inw), height(inh){}

	void Draw();
	void Draw(int color);

	void SetCenter(float inx, float iny){
		pos.x = inx - width / 2;
		pos.y = iny - height / 2;
	}
	void SetCenter(const Vector2& inpos){
		pos.x = inpos.x - width / 2;
		pos.y = inpos.y - height / 2;
	}

	void SetRect()
	{

	}

};