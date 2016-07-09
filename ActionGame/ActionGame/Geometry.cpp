#include"Geometry.h"
#include<DxLib.h>
#include<math.h>

void
Rect::Draw()
{
	DrawBox(pos.x, pos.y, pos.x + width, pos.y + height, 0xffffff, false);

}
void
Rect::Draw(int color)
{
	DrawBox(pos.x, pos.y, pos.x + width, pos.y + height, color, false);
}

float
Vector2::Length()
{
	return hypotf(x, y);
}

Vector2&
Vector2::Normalize()
{
	return Vector2(x / Length(), y / Length());
}

void
Vector2::Init()
{
	x = y = 0;
}