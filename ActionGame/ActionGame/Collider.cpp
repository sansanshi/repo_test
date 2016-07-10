#include "Collider.h"

Collider::Collider()
{
}


Collider::~Collider()
{
}

//Rect
//Collider::GetRect()
//{
//	return _rect;
//}

void
Collider::SetRect(Rect rc)
{
	pos = rc.pos;
	width = rc.width;
	height = rc.height;
}

void
Collider::ToDisable()
{
	_isCollidable = false;
}
void
Collider::ToEnable()
{
	_isCollidable = true;
}

void
Collider::SetCenter_Cam(Vector2 v, float f)
{
	SetCenter(Vector2(v.x + f, v.y));
}