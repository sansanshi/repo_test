#pragma once
#include"Geometry.h"
#include<math.h>
#include"Collider.h"
#include"GameObject.h"
class CollisionDetector
{
public:
	CollisionDetector();
	~CollisionDetector();

	static bool
		IsHit(Rect& rcA, Rect& rcB)
	{
			return !(rcA.Right() < rcB.Left() || rcB.Right() < rcA.Left() || rcA.Bottom()<rcB.Top() || rcB.Bottom()<rcA.Top());
	}

	/*static bool
	HitCheck(Collider* colA, Collider* colB)
	{
			if (IsHit(colA->GetRect(), colB->GetRect()))
			{
				colA->OnCollided(colB);
				colB->OnCollided(colA);
				return true;
			}
			else
			{
				return false;
			}
	}*/
	static bool
		HitCheck(GameObject* objA, GameObject* objB)
	{
			if (IsHit(objA->GetCollider(), objB->GetCollider()))
			{
				objA->OnCollided(objB);
				objB->OnCollided(objA);
				return true;
			}
			else
			{
				return false;
			}

	}
	static bool
		HitCheck(GameObject* objA, std::shared_ptr<GameObject> objB)//�v���C���[(objA)�ƃG�l�~�[(objB)
	{
			if (IsHit(objA->GetCollider(), objB->GetCollider()))
			{
				objA->OnCollided(objB);
				objB->OnCollided(objA);
				return true;
			}
			else
			{
				return false;
			}
	}

	//CollisionDetector�ɓ����
	//��`���d�Ȃ��������̒��S��Ԃ�
	//�������𐳂����s�����߂ɂ��̏�����Ishit��true�̂Ƃ��̂݌Ăяo��
	//2�̋�`�̂���Top�͂���ɂ�����@Bottom�͂�艺�ɂ�����@Left�͂��E�ɂ�����@Right�͂�荶�ɂ�������g���ďd�Ȃ��������̋�`�����߂�
	static Vector2 CenterOfHit(Rect& rc1, Rect& rc2)
	{
		return(RectOfOverlapped(rc1, rc2).Center());
	}

	//�d�Ȃ��������̋�`��Ԃ�
	static Rect RectOfOverlapped(Rect& rc1, Rect& rc2)
	{
		float left, right;
		float top, bottom;
		Rect ret;
		left = fmaxf(rc1.Left(), rc2.Left());
		right = fminf(rc1.Right(), rc2.Right());
		top = fmaxf(rc1.Top(), rc2.Top());
		bottom = fminf(rc1.Bottom(), rc2.Bottom());
		ret.pos = { left, top };
		ret.width = right - left;
		ret.height = bottom - top;
		return ret;
	}
};
