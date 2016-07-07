#include "Collider.h"

Collider::Collider()
{
}


Collider::~Collider()
{
}

Rect
Collider::GetRect()
{
	return _rect;
}

void
Collider::SetRect(Rect rc)
{
	pos = rc.pos;
	width = rc.width;
	height = rc.height;
}